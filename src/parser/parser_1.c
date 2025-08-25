/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:07:20 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 18:07:21 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Start Parsing Functions*/

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
	if (parser_f_loop(par, min_bp, &l_node, &r_node) == EXIT_FAILURE)
		return (NULL);
	return (l_node);
}

t_ast	*parse_simple_command(t_parser *par)
{
	t_command	c;

	init_command_structure(&c);
	while (par->curr_token && !is_operator_token(par->curr_token->type))
	{
		if (parse_simple_command_paren_check(par, &c) == EXIT_FAILURE)
			return (NULL);
		else if (is_redirect_token(par->curr_token->type))
		{
			if (parse_simple_command_red_token(par, &c) == EXIT_FAILURE)
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

int	parser_f_loop(t_parser *p, int min_bp, t_ast **ln, t_ast **rn)
{
	t_bp		bp;

	while (1)
	{
		if (p->curr_token == NULL || p->curr_token->type == CHAR_CPAREN)
			break ;
		else if (parser_function_loop_inval_token(p, ln) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		infix_binding_power(p->curr_token->type, &bp);
		if (bp.l != -1 && bp.r != -1)
		{
			if (bp.l < min_bp)
				break ;
			if (par_tree_agg(p, &bp, ln, rn))
				return (EXIT_FAILURE);
			continue ;
		}
		break ;
	}
	return (EXIT_SUCCESS);
}

void	ast_token_next(t_parser *par)
{
	par->curr_token = par->curr_token->next;
}

t_parser	*init_parser(t_lexer *lex)
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
