/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:07:09 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 18:07:10 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ast_node_addback(t_ast *l_node, t_token *token)
{
	t_ast	*first_l_node;

	if (l_node->left == NULL)
	{
		l_node->left = create_ast_node(token->type, token->data);
		return ;
	}
	first_l_node = l_node->left;
	while (l_node->left != NULL)
		l_node = l_node->left;
	l_node->left = create_ast_node(token->type, token->data);
	l_node = first_l_node;
}

void	ast_node_placeback_aux_right(t_ast **node_root, t_ast *node_toadd)
{
	t_ast	*first_node;

	if ((*node_root)->right == NULL)
	{
		(*node_root)->right = node_toadd;
		return ;
	}
	first_node = (*node_root)->right;
	while ((*node_root)->right->right)
		(*node_root)->right = (*node_root)->right->right;
	(*node_root)->right->right = node_toadd;
	(*node_root)->right = first_node;
}

void	ast_node_placeback(t_ast **n_root, t_ast *n_toadd, int side)
{
	t_ast	*first_node;

	if (*n_root == NULL)
	{
		*n_root = n_toadd;
		return ;
	}
	if (side == LEFT)
	{
		if ((*n_root)->left == NULL)
		{
			(*n_root)->left = n_toadd;
			return ;
		}
		first_node = (*n_root)->left;
		while ((*n_root)->left->left)
			(*n_root)->left = (*n_root)->left->left;
		(*n_root)->left->left = n_toadd;
		(*n_root)->left = first_node;
	}
	else
		ast_node_placeback_aux_right(n_root, n_toadd);
}

void	infix_binding_power(int type, t_bp *bp)
{
	if (type == CHAR_AND || type == CHAR_OR)
	{
		bp->l = 3;
		bp->r = 4;
	}
	else if (type == CHAR_PIPE)
	{
		bp->l = 5;
		bp->r = 6;
	}
	else
	{
		bp->l = -1;
		bp->r = -1;
	}
}

void	init_command_structure(t_command *c)
{
	c->file = NULL;
	c->cmd = NULL;
	c->redi = NULL;
	c->redi_root = NULL;
	c->token_prev = NULL;
}
