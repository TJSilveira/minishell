/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                              :+:      :+:    :+: */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 14:13:10 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 14:13:27 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ast_to_sexpr(t_ast *node)
{
	if (!node)
		return ;
	if (!node->left && !node->right)
	{
		if (node->data)
			printf("%s", node->data);
		return ;
	}
	printf("(");
	if (node->data)
		printf("%s", node->data);
	if (node->left)
	{
		printf(" ");
		ast_to_sexpr(node->left);
	}
	if (node->right)
	{
		printf(" ");
		ast_to_sexpr(node->right);
	}
	printf(")");
}

void	print_ast_sexpr(t_ast *root)
{
	if (!root)
	{
		printf("()\n");
		return ;
	}
	ast_to_sexpr(root);
	printf("\n");
}
