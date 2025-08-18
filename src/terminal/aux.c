/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:15:24 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 18:15:25 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_nodes(t_ast *root_tree)
{
	int	total;

	total = 0;
	if (root_tree == NULL)
		return (0);
	total++;
	total += count_nodes(root_tree->left);
	total += count_nodes(root_tree->right);
	return (total);
}

int	count_paren(t_lexer *lexer)
{
	int		total;
	int		i;
	t_token	*curr_tok;

	total = 0;
	i = 0;
	if (lexer == NULL || lexer->first_token == NULL)
		return (0);
	curr_tok = lexer->first_token;
	while (i < lexer->count_token)
	{
		if (curr_tok->type == CHAR_OPAREN || curr_tok->type == CHAR_CPAREN)
			total++;
		curr_tok = curr_tok->next;
		i++;
	}
	return (total);
}
