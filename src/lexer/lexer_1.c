/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:33:40 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:33:41 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	lexer_function(char *input, t_lexer *lexer)
{
	t_token_aux	aux;

	if (check_only_terminal(input) == 1)
	{
		lexer->first_token = NULL;
		lexer->count_token = 0;
		return (EXIT_SUCCESS);
	}
	if (check_matching_quotes(input) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (!lexer)
		return (EXIT_FAILURE);
	init_lexer_aux(input, &aux, lexer);
	process_char(input, &aux, lexer);
	clean_last_tokens(&aux, lexer);
	token_expansion(&aux, lexer);
	remove_empty_tokens(lexer);
	if (check_matching_parenthesis(lexer) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	handle_def_1char(char *i, t_token_aux *aux, t_lexer *l, int *f)
{
	if (*f == 0 && (i[aux->i] == CHAR_PIPE || i[aux->i] == CHAR_OUTRED
			|| i[aux->i] == CHAR_INRED || i[aux->i] == CHAR_AMPERSAND
			|| i[aux->i] == CHAR_OPAREN || i[aux->i] == CHAR_CPAREN))
	{
		if (aux->j != 0)
		{
			aux->curr_token->data[aux->j] = 0;
			aux->j = 0;
			aux->curr_token = add_token_back(l, aux->len_input);
		}
		aux->curr_token->data[aux->j] = i[aux->i];
		aux->curr_token->data[aux->j + 1] = 0;
		aux->curr_token->type = i[aux->i];
		aux->curr_token = add_token_back(l, aux->len_input);
		(*f)++;
	}
}

void	handle_def_2char_aux(char *input, t_token_aux *aux, int *f)
{
	if (input[aux->i] == '>' && input[aux->i + 1] == '>')
	{
		aux->curr_token->type = CHAR_APPEND;
		(*f)++;
	}
	else if (input[aux->i] == '<' && input[aux->i + 1] == '<')
	{
		aux->curr_token->type = CHAR_HEREDOC;
		(*f)++;
	}
	else if (input[aux->i] == '&' && input[aux->i + 1] == '&')
	{
		aux->curr_token->type = CHAR_AND;
		(*f)++;
	}
	else if (input[aux->i] == '|' && input[aux->i + 1] == '|')
	{
		aux->curr_token->type = CHAR_OR;
		(*f)++;
	}
}

void	handle_def_2char(char *i, t_token_aux *aux, t_lexer *l, int *f)
{
	handle_def_2char_aux(i, aux, f);
	if ((*f) == 1)
	{
		if (aux->j != 0)
		{
			aux->curr_token->data[aux->j] = 0;
			aux->j = 0;
			aux->curr_token = add_token_back(l, aux->len_input);
		}
		aux->curr_token->data[aux->j] = i[(aux->i)++];
		aux->curr_token->data[aux->j + 1] = i[aux->i];
		aux->curr_token = add_token_back(l, aux->len_input);
	}
}

void	handle_terminal(char *i, t_token_aux *aux, t_lexer *l, int *f)
{
	if (*f == 0 && (i[aux->i] == CHAR_SPACE
			|| i[aux->i] == CHAR_NEWLINE
			|| i[aux->i] == CHAR_TAB))
	{
		if (aux->j != 0)
		{
			aux->curr_token->data[aux->j] = 0;
			aux->j = 0;
			aux->curr_token = add_token_back(l, aux->len_input);
		}
		(*f)++;
	}
}
