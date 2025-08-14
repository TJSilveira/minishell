#include "../includes/minishell.h"

/* Start of AUX functions */

void	print_ast_node(t_ast *node)
{
	if (node == NULL)
		return ;
	printf("===============\n");
	printf("Node address: %p\n", node);
	printf("Node type: %i\n", node->type);
	printf("Node content: %s\n", node->data);
	printf("left node: %p\n", node->left);
	printf("right node: %p\n", node->right);
	printf("===============\n\n");
}

void	print_ast_sexpr(t_ast *root)
{
	if (!root)
	{
		printf("()\n");
		return ;
	}
	ast_to_sexpr(root);
	printf("\n");
}

void	print_node_leafs(t_ast *node)
{
	t_ast	*l_node;
	t_ast	*r_node;

	print_ast_node(node);
	l_node = node->left;
	r_node = node->right;
	while (l_node != NULL)
	{
		printf("This is a left node: %s\n", l_node->data);
		l_node = l_node->left;
	}
	while (r_node != NULL)
	{
		printf("This is a left node: %s\n", r_node->data);
		r_node = r_node->left;
	}
}

void	ast_to_sexpr(t_ast *node)
{
	if (!node)
		return ;
	if (!node->left && !node->right)
	{
		if (node->data)
			printf("%s", node->data);
		return ;
	}
	printf("(");
	if (node->data)
		printf("%s", node->data);
	if (node->left)
	{
		printf(" ");
		ast_to_sexpr(node->left);
	}
	if (node->right)
	{
		printf(" ");
		ast_to_sexpr(node->right);
	}
	printf(")");
}

t_ast	*create_ast_node(int type, char *content)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->data = ft_strdup(content);
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast	*create_ast_structure(t_token *token, t_ast *l_node, t_ast *r_node)
{
	t_ast	*node;

	node = create_ast_node(token->type, token->data);
	node->left = l_node;
	node->right = r_node;
	return (node);
}

int	is_default_token(int type)
{
	if (type == CHAR_DEF
		|| type == CHAR_QUOTE
		|| type == CHAR_DQUOTE)
	{
		return (1);
	}
	else
		return (0);
}

int	is_redirect_token(int type)
{
	if (type == CHAR_INRED
		|| type == CHAR_OUTRED
		|| type == CHAR_APPEND
		|| type == CHAR_HEREDOC)
	{
		return (1);
	}
	else
		return (0);
}

int	is_operator_token(int type)
{
	if (type == CHAR_PIPE
		|| type == CHAR_AND
		|| type == CHAR_OR)
	{
		return (1);
	}
	else
		return (0);
}

void	ast_node_addback(t_ast *l_node, t_token *token)
{
	t_ast	*first_l_node;

	if (l_node->left == NULL)
	{
		l_node->left = create_ast_node(token->type, token->data);
		return ;
	}
	first_l_node = l_node->left;
	while (l_node->left != NULL)
		l_node = l_node->left;
	l_node->left = create_ast_node(token->type, token->data);
	l_node = first_l_node;
}

void	ast_node_placeback_aux_right(t_ast **node_root, t_ast *node_toadd)
{
	t_ast	*first_node;

	if ((*node_root)->right == NULL)
	{
		(*node_root)->right = node_toadd;
		return ;
	}
	first_node = (*node_root)->right;
	while ((*node_root)->right->right)
		(*node_root)->right = (*node_root)->right->right;
	(*node_root)->right->right = node_toadd;
	(*node_root)->right = first_node;
}

void	ast_node_placeback(t_ast **node_root, t_ast *node_toadd, int side)
{
	t_ast	*first_node;

	if (*node_root == NULL)
	{
		*node_root = node_toadd;
		return ;
	}
	if (side == LEFT)
	{
		if ((*node_root)->left == NULL)
		{
			(*node_root)->left = node_toadd;
			return ;
		}
		first_node = (*node_root)->left;
		while ((*node_root)->left->left)
			(*node_root)->left = (*node_root)->left->left;
		(*node_root)->left->left = node_toadd;
		(*node_root)->left = first_node;
	}
	else
		ast_node_placeback_aux_right(node_root, node_toadd);
}

/* End of AUX functions */

/* Start of Free functions */

void	free_ast(t_ast *root)
{
	if (root)
	{
		free_ast(root->left);
		free_ast(root->right);
		free(root->data);
		free(root);
	}
}

void	free_parser_struct(t_parser *par)
{
	if (par)
	{
		free(par->root);
		free(par);
	}
}

/* End of Free functions*/

/* Start Parsing Functions*/

void	ast_token_next(t_parser *par)
{
	par->curr_token = par->curr_token->next;
}

t_parser	*init_paser(t_lexer *lex)
{
	t_parser	*par;

	par = malloc(sizeof(t_parser));
	par->root = malloc(sizeof(t_ast *));
	if (par == NULL || par->root == NULL)
		return (NULL);
	par->initial_token = lex->first_token;
	par->curr_token = lex->first_token;
	return (par);
}

void	infix_binding_power(int type, t_bp *bp)
{
	if (type == CHAR_AND || type == CHAR_OR)
	{
		bp->l = 3;
		bp->r = 4;
	}
	else if (type == CHAR_PIPE)
	{
		bp->l = 5;
		bp->r = 6;
	}
	else
	{
		bp->l = -1;
		bp->r = -1;
	}
}

void	init_command_structure(t_command *c)
{
	c->file = NULL;
	c->cmd = NULL;
	c->redi = NULL;
	c->redi_root = NULL;
	c->token_prev = NULL;
}

int	parse_simple_command_redirect_token(t_parser *par, t_command *c)
{
	c->token_redirect = par->curr_token;
	par->curr_token = par->curr_token->next;
	if (!is_default_token(par->curr_token->type))
	{
		ft_putstr_fd("Error: syntax error near unexpecter token `newline'\n",
			STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	c->file = create_ast_node(CHAR_DEF, par->curr_token->data);
	c->redi = create_ast_structure(c->token_redirect, NULL, c->file);
	ast_node_placeback(&c->redi_root, c->redi, RIGHT);
	c->token_prev = par->curr_token;
	par->curr_token = par->curr_token->next;
	return (EXIT_SUCCESS);
}

void	parse_simple_command_default_token(t_parser *par, t_command *c)
{
	if (c->cmd == NULL)
		c->cmd = create_ast_node(CHAR_DEF, par->curr_token->data);
	else
		ast_node_addback(c->cmd, par->curr_token);
	c->token_prev = par->curr_token;
	par->curr_token = par->curr_token->next;
}

void	parse_simple_command_oparen_token(t_parser *par, t_command *c)
{
	par->curr_token = par->curr_token->next;
	c->cmd = parser_function(par, 0);
	par->curr_token = par->curr_token->next;
}

int	parse_simple_command_parenthesis_check(t_parser *par, t_command	*c)
{
	if (c->token_prev && (is_default_token(c->token_prev->type)
			|| is_redirect_token(c->token_prev->type))
		&& par->curr_token->type == CHAR_OPAREN)
	{
		ft_putstr_fd("syntax error near unexpected token `('\n",
			STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	parse_simple_command_tree_aggregation(t_command	*c)
{
	if (c->cmd == NULL && c->redi_root != NULL)
		c->cmd = c->redi_root;
	else if (c->cmd != NULL && c->redi_root != NULL)
		c->cmd->right = c->redi_root;
}

t_ast	*parse_simple_command(t_parser *par)
{
	t_command	c;

	init_command_structure(&c);
	while (par->curr_token && !is_operator_token(par->curr_token->type))
	{
		if (parse_simple_command_parenthesis_check(par, &c) == EXIT_FAILURE)
			return (NULL);
		else if (is_redirect_token(par->curr_token->type))
		{
			if (parse_simple_command_redirect_token(par, &c) == EXIT_FAILURE)
				return (NULL);
		}
		else if (is_default_token(par->curr_token->type))
			parse_simple_command_default_token(par, &c);
		else if (par->curr_token->type == CHAR_OPAREN)
		{
			parse_simple_command_oparen_token(par, &c);
			return (c.cmd);
		}
		else
			break ;
	}
	parse_simple_command_tree_aggregation(&c);
	return (c.cmd);
}

int	parser_function_loop_inval_token(t_parser *par, t_ast **l_node)
{
	if ((is_default_token(par->curr_token->type)
			|| is_redirect_token(par->curr_token->type))
		&& ((*l_node)->type == CHAR_AND || (*l_node)->type == CHAR_OR))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `",
			STDERR_FILENO);
		ft_putstr_fd(par->curr_token->data, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	par_fun_loop_tree_agg(t_parser *par, t_bp *bp, t_ast **l_n, t_ast **r_n)
{
	t_token		*op;

	op = par->curr_token;
	par->curr_token = par->curr_token->next;
	if (par->curr_token == NULL || par->curr_token->type == CHAR_CPAREN)
	{
		ft_putstr_fd("Error: Binary operator missing right operand\n",
			STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	*r_n = parser_function(par, bp->r);
	*l_n = create_ast_structure(op, *l_n, *r_n);
	return (EXIT_SUCCESS);
}

int	parser_function_loop(t_parser *par, int min_bp, t_ast **l_n, t_ast **r_n)
{
	t_bp		bp;

	while (1)
	{
		if (par->curr_token == NULL || par->curr_token->type == CHAR_CPAREN)
			break ;
		else if (parser_function_loop_inval_token(par, l_n) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		infix_binding_power(par->curr_token->type, &bp);
		if (bp.l != -1 && bp.r != -1)
		{
			if (bp.l < min_bp)
				break ;
			if (par_fun_loop_tree_agg(par, &bp, l_n, r_n))
				return (EXIT_FAILURE);
			continue ;
		}
		break ;
	}
	return (EXIT_SUCCESS);
}

t_ast	*parser_function(t_parser *par, int min_bp)
{
	t_ast		*l_node;
	t_ast		*r_node;

	if (par == NULL || par->initial_token == NULL)
		return (NULL);
	l_node = NULL;
	r_node = NULL;
	l_node = parse_simple_command(par);
	if (l_node == NULL)
		return (NULL);
	if (parser_function_loop(par, min_bp, &l_node, &r_node) == EXIT_FAILURE)
		return (NULL);
	return (l_node);
}

/* End Parsing functions */