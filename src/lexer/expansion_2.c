/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:32:58 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:32:59 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	insert_expansion_aux(t_token *token, t_expansion *e, char *mid_str)
{
	int		i;
	int		j;

	i = -1;
	while (e->start_str[++i])
		e->final_str[i] = e->start_str[i];
	j = i;
	i = -1;
	while (mid_str[++i])
		e->final_str[i + j] = mid_str[i];
	j += i;
	i = -1;
	while (e->end_str[++i])
		e->final_str[i + j] = e->end_str[i];
	e->final_str[e->len_final] = 0;
	free(e->start_str);
	free(mid_str);
	free(e->end_str);
	free(token->data);
	token->data = e->final_str;
}

void	insert_expansion(t_token *t, int sta, int len, char *mid_str)
{
	t_expansion	e;

	e.len_str = ft_strlen(t->data);
	e.start_str = ft_substr(t->data, 0, sta);
	e.end_str = ft_substr(t->data, sta + len, e.len_str - (sta + len));
	e.len_final = 0;
	e.len_final += ft_strlen(e.start_str);
	e.len_final += ft_strlen(mid_str);
	e.len_final += ft_strlen(e.end_str);
	e.final_str = malloc(sizeof(char) * (e.len_final + 1));
	insert_expansion_aux(t, &e, mid_str);
}

int	token_quote_rmv(t_token *token, int *i, int *stat, int q_type)
{
	if (token->data[*i] == q_type && *stat == CHAR_DEF)
	{
		*stat = q_type;
		ft_rmvchr(token->data, &token->data[*i]);
		return (1);
	}
	else if (token->data[*i] == '\\' && token->data[*i + 1] == q_type
		&& *stat == q_type)
	{
		ft_rmvchr(token->data, &token->data[*i]);
		(*i)++;
		return (1);
	}
	else if (token->data[*i] == q_type && *stat == q_type)
	{
		*stat = CHAR_DEF;
		ft_rmvchr(token->data, &token->data[*i]);
		return (1);
	}
	return (0);
}

void	token_expansion_aux_ifs_while(t_token *t, int *i, int *j)
{
	while (t->data[*i + 1 + *j] != 0 && t->data[*i + 1 + *j] != ' '
		&& t->data[*i + 1 + *j] != '\"' && t->data[*i + 1 + *j] != '\''
		&& t->data[*i + 1 + *j] != '\\')
		(*j)++;
}

void	token_exp_aux_ifs(t_token *t, int *status, t_global *g, int *i)
{
	int		j;
	char	*temp;
	char	*to_expand;
	int		len;

	j = 0;
	if (t->data[*i] == CHAR_DOLLAR && t->data[*i + 1] != 0
		&& t->data[*i + 1] == '?' && *status != CHAR_QUOTE)
	{
		temp = ft_itoa(g->exit_code);
		len = ft_strlen(temp);
		insert_expansion(t, *i, len + 1, temp);
	}
	else if (t->data[*i] == CHAR_DOLLAR
		&& t->data[*i + 1] != 0 && t->data[*i + 1] != ' '
		&& t->data[*i + 1] != CHAR_DQUOTE && *status != CHAR_QUOTE)
	{
		token_expansion_aux_ifs_while(t, i, &j);
		temp = ft_substr(t->data, *i + 1, j);
		to_expand = find_ev(temp);
		free(temp);
		insert_expansion(t, *i, j + 1, to_expand);
	}
}
