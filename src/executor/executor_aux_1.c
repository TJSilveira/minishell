/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_aux_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:17:04 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:17:05 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	error_handler_efault(char *file_name)
{
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (file_name)
		free(file_name);
	exit(126);
}

void	error_handler_eacces(char *file_name)
{
	struct stat	file_stat;

	if (stat(file_name, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
	{
		ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		free(file_name);
		exit(126);
	}
	else
	{
		ft_putstr_fd("Permission denied\n", STDERR_FILENO);
		free(file_name);
		exit(126);
	}
}

void	error_handler_enoent(char *file_name)
{
	if (ft_strchr(file_name, '/'))
		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
	else
		ft_putstr_fd("command not found\n", STDERR_FILENO);
	free(file_name);
	exit(127);
}

void	error_handler(char *msg, char *f_name, int err_code, t_px *px)
{
	free_px_fds(px);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (f_name)
	{
		ft_putstr_fd(f_name, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (msg)
	{
		ft_putstr_fd(msg, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	if (err_code != -1)
		exit(err_code);
	if (errno == EFAULT)
		error_handler_efault(f_name);
	else if (errno == EACCES)
		error_handler_eacces(f_name);
	else if (errno == ENOENT && f_name)
		error_handler_enoent(f_name);
}

void	malloc_error_handler(void *ptr, int error_code)
{
	if (!ptr)
		error_handler("Error with malloc", NULL, error_code, NULL);
}
