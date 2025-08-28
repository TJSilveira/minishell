/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:15:30 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:15:31 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Executes a subtree that is only composed out of a builtin function. */
int	executor_builtin_func_normal(t_px *px)
{
	int	exit_code;

	exit_code = redirections_setup(px->root_tree, px);
	if (exit_code == EXIT_SUCCESS)
		exit_code = builtin_fun(px->root_tree, NULL, TO_RETURN, px);
	dup2(px->fd_stdin, STDIN_FILENO);
	dup2(px->fd_stdout, STDOUT_FILENO);
	if (px->fd_stdin > 2)
		close(px->fd_stdin);
	if (px->fd_stdout > 2)
		close(px->fd_stdout);
	if (px->fd_org_stdin > 2)
		close(px->fd_org_stdin);
	return (exit_code);
}

int	executor_builtin_func_pipe(t_px *px)
{
	int	exit_code;

	exit_code = redirections_setup(px->root_tree, px);
	if (exit_code == EXIT_SUCCESS)
		exit_code = builtin_fun(px->root_tree, NULL, TO_EXIT, px);
	return (exit_code);
}
