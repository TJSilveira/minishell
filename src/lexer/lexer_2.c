/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:33:43 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:33:46 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_start_quote(char *input, t_token_aux *aux, int *f)
{
	if (*f == 0 && input[aux->i] == CHAR_DQUOTE)
	{
		aux->curr_token->data[aux->j] = input[aux->i];
		aux->status = input[aux->i];
		aux->j++;
		(*f)++;
	}
	else if (*f == 0 && input[aux->i] == CHAR_QUOTE)
	{
		aux->curr_token->data[aux->j] = input[aux->i];
		aux->status = input[aux->i];
		aux->j++;
		(*f)++;
	}
}

void	process_char_def(char *input, t_token_aux *aux, t_lexer *lexer)
{
	int	flag;

	flag = 0;
	handle_def_2char(input, aux, lexer, &flag);
	handle_def_1char(input, aux, lexer, &flag);
	handle_terminal(input, aux, lexer, &flag);
	handle_start_quote(input, aux, &flag);
	if (flag == 0)
	{
		aux->curr_token->data[aux->j] = input[aux->i];
		aux->curr_token->type = CHAR_DEF;
		aux->j++;
	}
}

void	process_char_quote(char *input, t_token_aux *aux)
{
	if (input[aux->i] == '\\' && input[aux->i + 1] == aux->status)
	{
		aux->curr_token->data[(aux->j)++] = input[aux->i];
		aux->curr_token->data[(aux->j)++] = input[aux->i + 1];
		aux->i += 2;
	}
	if (input[aux->i] == aux->status)
	{
		aux->curr_token->data[aux->j] = input[aux->i];
		aux->j++;
		aux->curr_token->type = CHAR_DEF;
		aux->status = CHAR_DEF;
	}
	else
	{
		aux->curr_token->data[aux->j] = input[aux->i];
		aux->curr_token->type = aux->status;
		aux->j++;
	}
}

void	process_char(char *input, t_token_aux *aux, t_lexer *lexer)
{
	while (input[aux->i])
	{
		if (aux->status == CHAR_DEF)
			process_char_def(input, aux, lexer);
		else
			process_char_quote(input, aux);
		aux->i++;
	}
}
