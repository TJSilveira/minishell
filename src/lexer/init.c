/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:32:46 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:32:48 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_lexer_aux(char *input, t_token_aux *aux, t_lexer *lexer)
{
	aux->i = 0;
	aux->j = 0;
	aux->len_input = ft_strlen(input);
	lexer->first_token = init_token(lexer->first_token, aux->len_input);
	lexer->count_token++;
	aux->curr_token = lexer->first_token;
	aux->status = CHAR_DEF;
}

t_token	*init_token(t_token *token, int len_input)
{
	token = malloc(sizeof(t_token));
	malloc_error_handler(token, 2);
	token->data = ft_calloc(len_input + 1, sizeof(char));
	token->next = NULL;
	token->type = 0;
	return (token);
}
