/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:15:21 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:15:23 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	executor_pipe(t_px *px, t_ast *root)
{
	int	pipe_fd[2];
	int	pids[2];
	int	status;

	status = 0;
	if (pipe(pipe_fd) != 0)
		error_handler("Laying down the pipe(s)", NULL, 1, NULL);
	pids[0] = fork();
	if (pids[0] == 0)
		executor_pipe_left(px, root, pipe_fd);
	ignore_signals();
	pids[1] = fork();
	if (pids[1] == 0)
		executor_pipe_right(px, root, pipe_fd);
	return (executor_pipe_return(pipe_fd, pids, &status));
}

int	execute_subshell(t_px *px, t_ast *root)
{
	t_px			px_subshell;
	int				exit_code;
	t_prompt_line	*pl;

	pl = to_prompt_line_struct();
	px_subshell.num_commands = count_number_commands(root);
	px_subshell.root_tree = root;
	px_subshell.fd_stdin = px->fd_stdin;
	px_subshell.fd_stdout = px->fd_stdout;
	if (px->fd_org_stdin > 0)
		px_subshell.fd_org_stdin = px->fd_org_stdin;
	exit_code = executor_aux(&px_subshell, root, IN_PIPE);
	free(pl->prompt);
	return (exit_code);
}

int	executor_return(int *status, int pid)
{
	ignore_signals();
	waitpid(pid, status, 0);
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

int	executor(t_px *px, t_ast *cmd_node, int pipe_op)
{
	int	status;
	int	pid;

	if (pipe_op == NO_PIPE && is_builtin(px->root_tree))
		return (executor_builtin_func_normal(px));
	pid = fork();
	status = 0;
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		child_signals();
		if (pipe_op == IN_PIPE && is_builtin(px->root_tree))
			return (executor_builtin_func_pipe(px));
		status = redirections_setup(cmd_node, px);
		if (status == EXIT_FAILURE)
			exit (EXIT_FAILURE);
		if (cmd_node->data == NULL || cmd_node->data[0] == 0)
			error_handler_no_command(px);
		if (is_default_token(cmd_node->type))
			exec_command(cmd_node, px);
	}
	return (executor_return(&status, pid));
}
