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

void	write_line_break(int fd, t_px *px, char *line, char *limitor)
{
	size_t			size;
	t_prompt_line	*pl;

	pl = to_prompt_line_struct();
	size = ft_strlen(limitor);
	if (size == ft_strlen(line) && ft_strncmp(limitor, line, size) == 0)
	{
		free(line);
		free(limitor);
		close(fd);
		get_next_line(-1);
		free_px(px);
		free_global_struct();
		free_struct_to_free();
		free(pl->prompt);
		exit(EXIT_SUCCESS);
	}
}

int	write_line(char *limit, int fd, t_px *px)
{
	char			*line;
	char			*limitor;

	limitor = ft_strjoin(limit, "\n");
	while (1)
	{
		write(px->fd_stdout, "> ", 2);
		line = get_next_line(0);
		write_line_break(fd, px, line, limitor);
		if (write(fd, line, ft_strlen(line)) == -1)
			error_handler("Writing lines", NULL, 1, NULL);
		free(line);
	}
	exit(EXIT_FAILURE);
}

int	heredoc(char *limit, t_px *px)
{
	int	pipe_fd[2];
	int	pid;

	if (pipe(pipe_fd) == -1)
		error_handler("Laying down the pipe(s)", NULL, 1, NULL);
	pid = fork();
	if (pid == -1)
		error_handler("Fork creation", NULL, 1, NULL);
	if (pid == 0)
	{
		close(pipe_fd[READ]);
		write_line(limit, pipe_fd[WRITE], px);
	}
	else
	{
		close(pipe_fd[WRITE]);
		waitpid(pid, NULL, 0);
		return (pipe_fd[READ]);
	}
	return (-1);
}
