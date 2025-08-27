/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:33:28 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:33:28 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_matching_parenthesis_aux(t_lexer *lexer, int *counter)
{
	t_token	*curr_token;
	int		i;

	curr_token = lexer->first_token;
	i = -1;
	while (++i < lexer->count_token)
	{
		if (curr_token->type == CHAR_CPAREN)
			(*counter)--;
		else if (curr_token->type == CHAR_OPAREN)
			(*counter)++;
		if ((*counter) < 0)
		{
			ft_putstr_fd("Error: Invalid Parenthesis\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		curr_token = curr_token->next;
	}
	return (EXIT_SUCCESS);
}

int	check_only_parenthesis(t_lexer *lexer)
{
	t_token			*temp;
	int				counter;
	t_prompt_line	*pl;

	counter = 0;
	temp = lexer->first_token;
	pl = to_prompt_line_struct();
	while (temp)
	{
		if (temp->type == CHAR_OPAREN || temp->type == CHAR_CPAREN)
			counter++;
		temp = temp->next;
	}
	if (counter == lexer->count_token && counter > 0
		&& !(counter == 4 && ft_strlen(pl->line) == 4))
	{
		ft_putstr_fd("syntax error near unexpected token `)'\n", STDERR_FILENO);
		return (2);
	}
	return (EXIT_SUCCESS);
}

int	check_matching_parenthesis(t_lexer *lexer)
{
	int		counter;

	counter = 0;
	if (check_matching_parenthesis_aux(lexer, &counter) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (counter != 0)
	{
		ft_putstr_fd("Error: Invalid Parenthesis\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (check_only_parenthesis(lexer));
}

int	check_only_terminal(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n')
			return (0);
		i++;
	}
	if (i == 0 && input[0] == 0)
		return (1);
	return (1);
}

void	lexer_struct_token_org_update(t_lexer *lexer)
{
	t_token	*curr;

	curr = lexer->first_token;
	while (curr)
	{
		if (ft_strchr(curr->data, '\"') != NULL
			|| ft_strchr(curr->data, '\'') != NULL)
			curr->type_org = CHAR_DQUOTE;
		else
			curr->type_org = curr->type;
		curr = curr->next;
	}
}
