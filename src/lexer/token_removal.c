/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:33:05 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:33:05 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	remove_single_token(t_lexer *lexer, t_token *to_remove)
{
	t_token	*temp;

	temp = lexer->first_token;
	if (!lexer)
		return ;
	if (lexer->first_token == to_remove)
	{
		lexer->first_token = lexer->first_token->next;
		free(temp->data);
		free(temp);
	}
	else
	{
		while (temp->next != to_remove)
			temp = temp->next;
		temp->next = to_remove->next;
		free(to_remove->data);
		free(to_remove);
	}
	lexer->count_token--;
}

void	remove_empty_tokens(t_lexer *lexer)
{
	t_token	*c;
	t_token	*prev;

	c = lexer->first_token;
	prev = NULL;
	while (c)
	{
		if (c->data[0] == 0
			&& !(c->type_org == CHAR_QUOTE || c->type_org == CHAR_DQUOTE))
		{
			remove_single_token(lexer, c);
			if (prev == NULL)
				c = lexer->first_token;
			else
				c = prev->next;
			continue ;
		}
		prev = c;
		c = c->next;
	}
}

void	clean_last_tokens(t_token_aux *aux, t_lexer *lexer)
{
	t_token		*temp;

	if (aux->curr_token->data[0] == 0 && aux->curr_token->type == 0)
	{
		temp = aux->curr_token;
		free(temp->data);
		free(temp);
		aux->curr_token = get_previous_token(lexer->first_token,
				aux->curr_token);
		aux->curr_token->next = NULL;
		lexer->count_token--;
	}
}
