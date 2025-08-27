/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:17:20 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:17:21 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_arrays(char **arrays)
{
	int	i;

	i = 0;
	while (arrays != NULL && arrays[i])
	{
		free(arrays[i]);
		i++;
	}
	free(arrays);
}

void	free_px(t_px *px)
{
	if (px)
	{
		if (px->fd_stdin > 2)
			close(px->fd_stdin);
		if (px->fd_stdout > 2)
			close(px->fd_stdout);
		if (px->fd_org_stdin > 2)
			close(px->fd_org_stdin);
		free(px);
		px = NULL;
	}
}

void	free_struct_to_free(void)
{
	t_to_free	*to_free;

	to_free = to_free_struct();
	if (to_free->lexer)
	{
		free_lexer(to_free->lexer);
		to_free->lexer = NULL;
	}
	if (to_free->root_tree)
	{
		free_ast(to_free->root_tree);
		to_free->root_tree = NULL;
	}
	if (to_free->par)
	{
		free_parser_struct(to_free->par);
		to_free->par = NULL;
	}
}
