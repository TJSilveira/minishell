/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:50:48 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 16:50:49 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_a_to_exitcode(const char *nptr, int *total)
{
	int	i;
	int	sign;

	i = 0;
	sign = 1;
	if (nptr[i] == '-')
	{
		sign *= -1;
		i++;
	}
	else if (nptr[i] == '+')
		i++;
	while (nptr[i])
	{
		if (nptr[i] >= '0' && nptr[i] <= '9')
			*total = *total * 10 + nptr[i] - '0';
		else
			return (EXIT_FAILURE);
		i++;
	}
	*total = *total * sign;
	*total = (unsigned char) *total;
	return (EXIT_SUCCESS);
}

int	exit_free_aux(int exit_code, t_px *px)
{
	free_global_struct();
	free_struct_to_free();
	free_px_fds(px);
	exit(exit_code);
}

void	exit_builtin_aux(t_ast *node, int *exit_code, t_px *px)
{
	if (ft_a_to_exitcode(node->data, exit_code))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(node->data, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit_free_aux(2, px);
	}
}

int	exit_arg_count(t_ast *node, int *exit_code)
{
	int				count;

	count = 0;
	if (node != NULL && ft_a_to_exitcode(node->data, exit_code)
		== EXIT_FAILURE)
		return (0);
	while (node)
	{
		count++;
		node = node->left;
	}
	if (count > 1)
	{
		ft_putstr_fd("exit\nminishell: exit: too many arguments\n",
			STDERR_FILENO);
	}
	return (count);
}

int	exit_builtin(t_ast *node, t_px *px)
{
	t_prompt_line	*pl;
	t_global		*global;

	pl = to_prompt_line_struct();
	global = global_struct();
	if (exit_arg_count(node, &global->exit_code) > 1)
		return (EXIT_FAILURE);
	if (node == NULL && pl->input_type == NONINTERACTIVE_MODE)
	{
		free_px_fds(px);
		exit(EXIT_SUCCESS);
	}
	if (pl->input_type == INTERACTIVE_MODE)
	{
		free(pl->prompt);
		rl_clear_history();
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	}
	if (node == NULL)
		exit_free_aux(global->exit_code, px);
	global->exit_code = 0;
	exit_builtin_aux(node, &global->exit_code, px);
	return (exit_free_aux(global->exit_code, px));
}
