/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:59:26 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:59:28 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast	*create_ast_node(int type, char *content)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->data = ft_strdup(content);
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast	*create_ast_structure(t_token *t, t_ast *l_node, t_ast *r_node)
{
	t_ast	*node;

	node = create_ast_node(t->type, t->data);
	node->left = l_node;
	node->right = r_node;
	return (node);
}

int	is_default_token(int type)
{
	if (type == CHAR_DEF
		|| type == CHAR_QUOTE
		|| type == CHAR_DQUOTE)
	{
		return (1);
	}
	else
		return (0);
}

int	is_redirect_token(int type)
{
	if (type == CHAR_INRED
		|| type == CHAR_OUTRED
		|| type == CHAR_APPEND
		|| type == CHAR_HEREDOC)
	{
		return (1);
	}
	else
		return (0);
}

int	is_operator_token(int type)
{
	if (type == CHAR_PIPE
		|| type == CHAR_AND
		|| type == CHAR_OR)
	{
		return (1);
	}
	else
		return (0);
}
