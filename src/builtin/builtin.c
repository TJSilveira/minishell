/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:49:28 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 16:49:31 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_fun(t_ast *node, char **comms, int to_exit, t_px *px)
{
	int				exit_code;
	t_prompt_line	*pl;

	pl = to_prompt_line_struct();
	exit_code = builtin_execution(node, px);
	if (exit_code == NO_BUILTIN)
		return (0);
	else if (to_exit == TO_EXIT)
	{
		free_arrays(comms);
		free_struct_to_free();
		free_global_struct();
		rl_clear_history();
		free(pl->prompt);
		exit(exit_code);
	}
	else
		return (exit_code);
}

int	is_builtin(t_ast *n)
{
	if ((ft_strncmp("echo", n->data, 4) == 0 && ft_strlen(n->data) == 4)
		|| (ft_strncmp("cd", n->data, 2) == 0 && ft_strlen(n->data) == 2)
		|| (ft_strncmp("pwd", n->data, 3) == 0 && ft_strlen(n->data) == 3)
		|| (ft_strncmp("export", n->data, 6) == 0 && ft_strlen(n->data) == 6)
		|| (ft_strncmp("unset", n->data, 5) == 0 && ft_strlen(n->data) == 5)
		|| (ft_strncmp("env", n->data, 3) == 0 && ft_strlen(n->data) == 3)
		|| (ft_strncmp("exit", n->data, 4) == 0 && ft_strlen(n->data) == 4))
		return (1);
	else
		return (0);
}

int	builtin_execution(t_ast *n, t_px *px)
{
	if (ft_strncmp("echo", n->data, 4) == 0 && ft_strlen(n->data) == 4)
		return (echo_builtin(n->left));
	else if (ft_strncmp("cd", n->data, 2) == 0 && ft_strlen(n->data) == 2)
		return (cd_builtin(n->left));
	else if (ft_strncmp("pwd", n->data, 3) == 0 && ft_strlen(n->data) == 3)
		return (pwd_builtin());
	else if (ft_strncmp("export", n->data, 6) == 0 && ft_strlen(n->data) == 6)
		return (export_builtin(n->left));
	else if (ft_strncmp("unset", n->data, 5) == 0 && ft_strlen(n->data) == 5)
		return (unset_builtin(n->left));
	else if (ft_strncmp("env", n->data, 3) == 0 && ft_strlen(n->data) == 3)
		return (env_builtin(n->left));
	else if (ft_strncmp("exit", n->data, 4) == 0 && ft_strlen(n->data) == 4)
		return (exit_builtin(n->left, px));
	else
		return (NO_BUILTIN);
}
