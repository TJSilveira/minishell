/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:15:07 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:15:08 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	executor_aux(t_px *px, t_ast *root)
{
	int	exit_code;

	if (root == NULL)
		return (EXIT_SUCCESS);
	if (is_default_token(root->type))
		return (executor(px, root));
	else if (root->type == CHAR_PIPE)
		return (executor_pipe(px, root));
	else if (root->type == CHAR_AND)
	{
		exit_code = executor_aux(px, root->left);
		if (exit_code != 0)
			return (exit_code);
		return (executor_aux(px, root->right));
	}
	else if (root->type == CHAR_OR)
	{
		exit_code = executor_aux(px, root->left);
		if (exit_code == 0)
			return (exit_code);
		return (executor_aux(px, root->right));
	}
	return (EXIT_SUCCESS);
}

void	restore_fd(t_px *px)
{
	dup2(px->fd_stdin, STDIN_FILENO);
	dup2(px->fd_stdout, STDOUT_FILENO);
	close(px->fd_stdin);
	close(px->fd_stdout);
}

int	executor_pipe_left(t_px *px, t_ast *root, int pipe_fd[2])
{
	int	exit_code;

	child_signals();
	dup2(pipe_fd[WRITE], STDOUT_FILENO);
	close(pipe_fd[READ]);
	exit_code = execute_subshell(px, root->left);
	free_struct_to_free();
	free_global_struct();
	exit (exit_code);
}

int	executor_pipe_right(t_px *px, t_ast *root, int pipe_fd[2])
{
	int	exit_code;

	child_signals();
	dup2(pipe_fd[READ], STDIN_FILENO);
	close(pipe_fd[WRITE]);
	exit_code = execute_subshell(px, root->right);
	free_struct_to_free();
	free_global_struct();
	exit (exit_code);
}

int	executor_pipe_return(int pipe_fd[2], int pids[2], int *status)
{
	close(pipe_fd[READ]);
	close(pipe_fd[WRITE]);
	waitpid(pids[0], status, 0);
	if (WIFSIGNALED(*status))
		write(1, "\n", 1);
	waitpid(pids[1], status, 0);
	if (WIFEXITED(*status))
		return (WEXITSTATUS(*status));
	else if (WIFSIGNALED(*status))
	{
		if (WTERMSIG(*status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(*status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		return (WTERMSIG(*status) + 128);
	}
	return (EXIT_FAILURE);
}
