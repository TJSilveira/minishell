/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:14:27 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 18:14:28 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Checks whether env_to_change exists. If yes, change it to new_env. 
If not, add it to the end.*/

void	update_env_loop(char *ev_ch, char *new_env, t_global *global)
{
	int			count;
	char		*to_add;

	count = -1;
	while (global->ev[++count])
	{
		if (ft_strncmp(ev_ch, global->ev[count], ft_strlen(ev_ch)) == 0
			&& (global->ev[count][ft_strlen(ev_ch)] == 0
			|| global->ev[count][ft_strlen(ev_ch)] == '='))
			break ;
	}
	if (global->ev[count])
	{
		if (ft_strlen(new_env) != 0)
		{
			free(global->ev[count]);
			global->ev[count] = ft_strjoin(ev_ch, new_env);
		}
	}
	else
	{
		to_add = ft_strjoin(ev_ch, new_env);
		add_env(to_add);
		free(to_add);
	}
}

void	update_env(char *ev_ch, char *new_env, char *to_free)
{
	t_global	*global;
	char		*to_add;

	global = global_struct();
	if (global->ev == NULL)
	{
		to_add = ft_strjoin(ev_ch, new_env);
		add_env(to_add);
		free(to_add);
	}
	else
		update_env_loop(ev_ch, new_env, global);
	free(to_free);
}

void	remove_env_aux_2(int count, int *i, t_global *global)
{
	char		**new_env;
	int			j;

	new_env = malloc(sizeof(char *) * (*i));
	(*i) = -1;
	j = 0;
	while (global->ev[++(*i)])
	{
		if ((*i) != count)
			new_env[j++] = global->ev[(*i)];
		else
			free(global->ev[(*i)]);
	}
	new_env[(*i) - 1] = NULL;
	free(global->ev);
	global->ev = new_env;
}

void	remove_env_aux(int count)
{
	t_global	*global;
	int			i;

	global = global_struct();
	i = -1;
	while (global->ev[i])
		i++;
	if (i == 1)
	{
		free(global->ev[0]);
		free(global->ev);
		global->ev = NULL;
		return ;
	}
	remove_env_aux_2(count, &i, global);
}

void	remove_env(char *ev_rmv)
{
	t_global	*global;
	int			count;

	count = -1;
	global = global_struct();
	if (global->ev == NULL)
		return ;
	while (global->ev[++count])
	{
		if (ft_strncmp(ev_rmv, global->ev[count], ft_strlen(ev_rmv)) == 0)
			break ;
	}
	if (global->ev[count])
		remove_env_aux(count);
}
