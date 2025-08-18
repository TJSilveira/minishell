/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:33:24 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:33:25 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*get_last_token(t_lexer *lexer)
{
	t_token	*last_token;

	if (!lexer)
		return (NULL);
	if (lexer->first_token == NULL)
		return (lexer->first_token);
	last_token = lexer->first_token;
	while (last_token->next != NULL)
		last_token = last_token->next;
	return (last_token);
}

t_token	*get_previous_token(t_token *first_token, t_token *curr_token)
{
	t_token	*previous_token;

	if (!first_token)
		return (NULL);
	previous_token = first_token;
	while (previous_token->next != curr_token && previous_token->next != NULL)
		previous_token = previous_token->next;
	return (previous_token);
}

t_token	*add_token_back(t_lexer *lexer, int len_input)
{
	t_token	*last_token;
	t_token	*temp_token;

	if (lexer->first_token == NULL)
	{
		lexer->first_token = init_token(lexer->first_token, len_input);
		lexer->count_token++;
		return (lexer->first_token);
	}
	else
	{
		temp_token = get_last_token(lexer);
		last_token = init_token(lexer->first_token, len_input);
		temp_token->next = last_token;
		lexer->count_token++;
		return (last_token);
	}
}

void	check_matching_quotes_aux(char *input, int *counter)
{
	int		i;
	char	quote_type;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\\' && (input[i + 1] == '\'' || input[i + 1] == '\"'))
		{
			i += 2;
			continue ;
		}
		if (*counter == 0 && (input[i] == '\"' || input[i] == '\''))
		{
			(*counter)++;
			quote_type = input[i];
		}
		else if (*counter == 1 && input[i] == quote_type)
			*counter = 0;
		i++;
	}
}

int	check_matching_quotes(char *input)
{
	int		counter;

	counter = 0;
	check_matching_quotes_aux(input, &counter);
	if (counter > 0)
	{
		ft_putstr_fd("Error: Unclosed quotation detected\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
