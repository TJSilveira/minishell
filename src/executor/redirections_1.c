/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:15:49 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:15:51 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	redirections_files_setup(int fd, int type)
{
	if (fd == -1)
		return ;
	if (type == CHAR_INRED || type == CHAR_HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			error_handler("Duplicating read-end pipe to STDIN", NULL, 1, NULL);
		close(fd);
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			error_handler("Duplicating write-end pipe to STDOUT",
				NULL, 1, NULL);
		close(fd);
	}
}

int	redirections_setup(t_ast *root, t_px *px)
{
	int	fd;

	while (root)
	{
		if (is_redirect_token(root->type))
		{
			fd = open_fd(root->right->data, root->type, px);
			if (errno == EACCES || fd == -1)
			{
				restore_fd(px);
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(root->right->data, STDERR_FILENO);
				ft_putstr_fd(": ", STDERR_FILENO);
				ft_putstr_fd(strerror(errno), STDERR_FILENO);
				ft_putstr_fd("\n", STDERR_FILENO);
				return (EXIT_FAILURE);
			}
			redirections_files_setup(fd, root->type);
		}
		root = root->right;
	}
	return (EXIT_SUCCESS);
}
