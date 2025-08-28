/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:07:28 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 18:07:29 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		free_ast(*l_node);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	par_tree_agg(t_parser *par, t_bp *bp, t_ast **l_n, t_ast **r_n)
{
	t_token		*op;

	op = par->curr_token;
	par->curr_token = par->curr_token->next;
	if (par->curr_token == NULL || par->curr_token->type == CHAR_CPAREN)
	{
		ft_putstr_fd("Error: Binary operator missing right operand\n",
			STDERR_FILENO);
		free_ast(*l_n);
		return (EXIT_FAILURE);
	}
	*r_n = parser_function(par, bp->r);
	if (*r_n == NULL)
	{
		free_ast(*l_n);
		return (EXIT_FAILURE);
	}
	*l_n = create_ast_structure(op, *l_n, *r_n);
	return (EXIT_SUCCESS);
}

t_parser	*init_parser(t_lexer *lex)
{
	t_parser	*par;

	par = malloc(sizeof(t_parser));
	malloc_error_handler(par, 2);
	par->root = malloc(sizeof(t_ast *));
	malloc_error_handler(par->root, 2);
	if (par == NULL || par->root == NULL)
		return (NULL);
	par->initial_token = lex->first_token;
	par->curr_token = lex->first_token;
	return (par);
}
