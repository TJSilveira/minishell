/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:59:03 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:59:03 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parse_simple_command_red_token(t_parser *par, t_command *c)
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

int	parse_simple_command_paren_check(t_parser *par, t_command	*c)
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
