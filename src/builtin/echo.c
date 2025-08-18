/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:49:46 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 16:49:47 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	echo_builtin_aux(t_ast *node, int option)
{
	t_ast	*initial_node;
	int		count;

	initial_node = node;
	count = 0;
	while (node)
	{
		count++;
		node = node->left;
	}
	while (initial_node)
	{
		if (count > 1)
		{
			ft_putstr_fd(initial_node->data, STDOUT_FILENO);
			ft_putstr_fd(" ", STDOUT_FILENO);
		}
		else
			ft_putstr_fd(initial_node->data, STDOUT_FILENO);
		initial_node = initial_node->left;
		count--;
	}
	if (option == 0)
		ft_putstr_fd("\n", STDOUT_FILENO);
}

int	echo_builtin(t_ast *node)
{
	int		option;

	option = 0;
	if (node == NULL)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
	if (ft_strncmp(node->data, "-n", 2) == 0 && ft_strlen(node->data) == 2)
	{
		node = node->left;
		option = 1;
	}
	echo_builtin_aux(node, option);
	return (EXIT_SUCCESS);
}
