/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_aux_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:17:09 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:17:10 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**commands_extractor(t_ast *cmd_node)
{
	t_ast	*temp;
	int		count;
	char	**commands;

	if (cmd_node == NULL || cmd_node->data == NULL
		|| cmd_node->type != CHAR_DEF)
		return (NULL);
	temp = cmd_node;
	count = 0;
	while (temp)
	{
		count++;
		temp = temp->left;
	}
	commands = malloc(sizeof(char *) * (count + 1));
	temp = cmd_node;
	count = 0;
	while (temp)
	{
		commands[count] = ft_strdup(temp->data);
		temp = temp->left;
		count++;
	}
	commands[count] = NULL;
	return (commands);
}

char	**path_extractor(void)
{
	char		**paths;
	t_global	*global;
	int			i;

	global = global_struct();
	i = -1;
	while (global->ev != NULL && global->ev[++i])
	{
		if (ft_strncmp(global->ev[i], "PATH=", 4) == 0)
		{
			paths = ft_split(global->ev[i] + 5, ':');
			return (paths);
		}
	}
	return (NULL);
}

char	*ft_strjoin_3(const char *s1, char connector, const char *s2)
{
	int		i;
	int		j;
	char	*res;

	res = malloc((ft_strlen(s1) + 2 + ft_strlen(s2)) * sizeof(char));
	if (!res)
		error_handler("Malloc problem in ft_strjoin_3", NULL, 1, NULL);
	i = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	res[i] = connector;
	i++;
	j = 0;
	while (s2[j])
	{
		res[i + j] = s2[j];
		j++;
	}
	res[i + j] = 0;
	return (res);
}
