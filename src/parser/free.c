/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:59:09 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:59:10 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_ast(t_ast *root)
{
	if (root)
	{
		free_ast(root->left);
		free_ast(root->right);
		free(root->data);
		free(root);
	}
}

void	free_parser_struct(t_parser *par)
{
	if (par)
	{
		free(par->root);
		free(par);
	}
}
