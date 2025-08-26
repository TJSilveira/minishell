/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:49:55 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 16:49:56 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_env_builtin(void)
{
	t_global	*global;
	int			i;

	global = global_struct();
	i = 0;
	if (global->ev == NULL)
		return ;
	while (global->ev[i])
	{
		if (ft_strnstr(global->ev[i], "=", ft_strlen(global->ev[i])) != NULL)
		{
			ft_putstr_fd(global->ev[i], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		i++;
	}
}

int	env_builtin(t_ast *node)
{
	if (node == NULL)
	{
		print_env_builtin();
		return (EXIT_SUCCESS);
	}
	else
	{
		ft_putstr_fd("env: no options or arguments allowed\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
}
