/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:50:26 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 16:50:27 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	unset_builtin(t_ast *node)
{
	if (node == NULL)
		return (EXIT_SUCCESS);
	while (node)
	{
		remove_env(node->data);
		node = node->left;
	}
	return (EXIT_SUCCESS);
}
