/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:49:38 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 16:49:39 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	cd_builtin_no_env_variables(void)
{
	t_global	*global;

	global = global_struct();
	if (global->ev == NULL)
	{
		ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	cd_builtin_no_args_aux(char *buffer, char *home)
{
	if (chdir(home) == -1)
	{
		free(home);
		ft_putstr_fd("cd: No such file or directory\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	else
	{
		update_env("OLDPWD=", buffer, NULL);
		update_env("PWD=", home, home);
		return (EXIT_SUCCESS);
	}
}

int	cd_builtin_no_args(char *buffer)
{
	char	*home;

	if (cd_builtin_no_env_variables() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	home = find_ev("HOME");
	if (home[0] == 0)
	{
		free(home);
		ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	else
		return (cd_builtin_no_args_aux(buffer, home));
}

int	cd_builtin_1_arg(t_ast *initial_node, char *buffer)
{
	if (chdir(initial_node->data) == -1)
	{
		ft_putstr_fd("cd: No such file or directory\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	else
	{
		update_env("OLDPWD=", buffer, NULL);
		update_env("PWD=", initial_node->data, NULL);
		return (EXIT_SUCCESS);
	}
}

int	cd_builtin(t_ast *node)
{
	int		count;
	t_ast	*initial_node;
	char	buffer[1024];

	getcwd(buffer, 1024);
	initial_node = node;
	count = 0;
	while (node)
	{
		count++;
		node = node->left;
	}
	if (count == 0)
		return (cd_builtin_no_args(buffer));
	else if (count == 1)
		return (cd_builtin_1_arg(initial_node, buffer));
	ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
	return (EXIT_FAILURE);
}
