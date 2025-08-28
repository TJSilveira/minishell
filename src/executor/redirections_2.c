/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:15:55 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:15:56 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	open_fd(char *path, int option, t_px *px)
{
	int	fd;

	fd = -1;
	if (option == CHAR_INRED)
		fd = open(path, O_RDONLY);
	else if (option == CHAR_OUTRED)
		fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	else if (option == CHAR_HEREDOC)
		fd = heredoc(path, px);
	else if (option == CHAR_APPEND)
		fd = open(path, O_WRONLY | O_APPEND | O_CREAT, 0777);
	if (fd == -1 && CHAR_INRED == 'I')
		perror("Error: opening file");
	return (fd);
}

void	write_line_break(char *line, char *limitor, t_px *px)
{
	size_t			size;
	t_prompt_line	*pl;

	if (line == NULL)
		return ;
	pl = to_prompt_line_struct();
	size = ft_strlen(limitor);
	if (size + 1 == ft_strlen(line) && ft_strncmp(limitor, line, size) == 0
		&& (int)(ft_strchr(line, '\n') - line) == (int)(size))
	{
		free(line);
		get_next_line(0, TO_CLEAN);
		free_global_struct();
		free_struct_to_free();
		free(pl->prompt);
		free_px_fds(px);
		exit(EXIT_SUCCESS);
	}
}

int	write_line(char *limit, t_px *px)
{
	char			*line;

	while (1)
	{
		write(px->fd_stdout, "> ", 2);
		line = get_next_line(0, TO_USE);
		if (line == NULL)
		{
			get_next_line(0, TO_CLEAN);
			free_px_fds(px);
			exit(EXIT_SUCCESS);
		}
		write_line_break(line, limit, px);
		if (write(STDOUT_FILENO, line, ft_strlen(line)) == -1)
			error_handler("Writing lines", NULL, 1, px);
		free(line);
	}
	free_px_fds(px);
	exit(EXIT_FAILURE);
}

void	heredoc_child(char *limit, t_px *px, int pipe_fd[2])
{
	heredoc_signals();
	close(pipe_fd[READ]);
	dup2(px->fd_org_stdin, STDIN_FILENO);
	dup2(pipe_fd[WRITE], STDOUT_FILENO);
	close(pipe_fd[WRITE]);
	write_line(limit, px);
}

int	heredoc(char *limit, t_px *px)
{
	int	pipe_fd[2];
	int	pid;

	if (pipe(pipe_fd) == -1)
		error_handler("Laying down the pipe(s)", NULL, 1, px);
	pid = fork();
	if (pid == -1)
		error_handler("Fork creation", NULL, 1, px);
	if (pid == 0)
		heredoc_child(limit, px, pipe_fd);
	else
	{
		ignore_signals();
		close(pipe_fd[WRITE]);
		waitpid(pid, NULL, 0);
		parent_signals();
		return (pipe_fd[READ]);
	}
	return (-1);
}
