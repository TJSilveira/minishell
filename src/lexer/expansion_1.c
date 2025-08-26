/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:32:54 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:32:55 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	token_expansion_aux(t_token *token)
{
	int			i;
	t_global	*global;
	int			status;

	i = 0;
	global = global_struct();
	status = CHAR_DEF;
	while (token->data[i])
	{
		if (token_quote_rmv(token, &i, &status, CHAR_QUOTE))
			continue ;
		if (token_quote_rmv(token, &i, &status, CHAR_DQUOTE))
			continue ;
		token_exp_aux_ifs(token, &status, global, &i);
		if (token->data[i] == 0)
			break ;
		i++;
	}
}

void	token_expansion(t_token_aux *aux, t_lexer *lexer)
{
	if (lexer == NULL || lexer->first_token == NULL)
		return ;
	aux->curr_token = lexer->first_token;
	while (aux->curr_token)
	{
		if (aux->curr_token->type == CHAR_DQUOTE
			|| aux->curr_token->type == CHAR_DEF)
			token_expansion_aux(aux->curr_token);
		aux->curr_token = aux->curr_token->next;
	}
}
