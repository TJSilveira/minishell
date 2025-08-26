/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:50:11 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 16:50:12 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_export_builtin_aux(char **env_array)
{
	int		i;

	i = -1;
	while (env_array[++i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		print_with_dquotes(env_array[i]);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	free(env_array);
}

void	print_export_builtin(void)
{
	t_global	*global;
	char		**env_array;
	int			total_ev;
	int			i;

	global = global_struct();
	if (global->ev == NULL)
		return ;
	i = -1;
	total_ev = 0;
	while (global->ev[++i])
		total_ev++;
	env_array = (char **) malloc(sizeof(char *) * (total_ev + 1));
	i = -1;
	while (global->ev[++i])
		env_array[i] = global->ev[i];
	env_array[i] = NULL;
	quicksort_strings(env_array, 0, total_ev - 1);
	print_export_builtin_aux(env_array);
}

int	check_valid_export_aux(char *str)
{
	int	i;

	if (ft_isdigit(str[0]))
		return (EXIT_FAILURE);
	i = -1;
	while (str[++i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_valid_export(char *str)
{
	char	*es_pos;
	char	*env_to_change;

	if (str[0] == '=')
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd("=", STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	es_pos = ft_strchr(str, '=');
	if (es_pos != NULL)
		env_to_change = ft_substr(str, 0, es_pos - str);
	else
		env_to_change = str;
	if (check_valid_export_aux(env_to_change) == EXIT_FAILURE)
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(env_to_change, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		if (es_pos != NULL)
			free(env_to_change);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	export_builtin(t_ast *node)
{
	char	*es_pos;
	char	*env_to_change;
	char	*new_env;

	if (node == NULL)
		print_export_builtin();
	while (node)
	{
		if (check_valid_export(node->data) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		es_pos = ft_strchr(node->data, '=');
		if (es_pos != NULL)
		{
			env_to_change = ft_substr(node->data, 0, es_pos - node->data);
			new_env = ft_substr(node->data, es_pos - node->data,
					ft_strlen(node->data) - (es_pos - node->data));
			update_env(env_to_change, new_env, env_to_change);
			free(new_env);
		}
		else
			update_env(node->data, "", NULL);
		node = node->left;
	}
	return (EXIT_SUCCESS);
}
