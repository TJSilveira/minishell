/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:14:31 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 18:14:33 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_env_aux(char *to_add, t_global *global)
{
	int			count;
	char		**new_env;
	int			i;

	count = 0;
	while (global->ev[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	malloc_error_handler(new_env, 2);
	i = -1;
	while (++i < count)
		new_env[i] = global->ev[i];
	new_env[count] = ft_strdup(to_add);
	new_env[count + 1] = NULL;
	free(global->ev);
	global->ev = new_env;
}

void	add_env(char *to_add)
{
	t_global	*global;

	global = global_struct();
	if (global->ev == NULL)
	{
		global->ev = malloc(sizeof(char *) * (2));
		malloc_error_handler(global->ev, 2);
		global->ev[0] = ft_strdup(to_add);
		global->ev[1] = NULL;
		return ;
	}
	else
		add_env_aux(to_add, global);
}

char	*find_ev(char *to_expand)
{
	t_global	*global;
	int			i;
	char		*result;

	global = global_struct();
	i = -1;
	while (global->ev[++i])
	{
		if (ft_strncmp(to_expand, global->ev[i], ft_strlen(to_expand)) == 0
			&& (global->ev[i][ft_strlen(to_expand)]) == '=')
		{
			result = ft_substr(global->ev[i], ft_strlen(to_expand) + 1,
					ft_strlen(global->ev[i]));
			return (result);
		}
	}
	return (ft_strdup(""));
}
