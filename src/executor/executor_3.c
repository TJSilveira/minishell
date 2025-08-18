/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:15:30 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 17:15:31 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exec_command_free_aux(char **paths, char **commands, t_px *px)
{
	t_prompt_line	*pl;

	pl = to_prompt_line_struct();
	free_arrays(commands);
	free_arrays(paths);
	free_px(px);
	free_struct_to_free();
	free_global_struct();
	rl_clear_history();
	free(pl->prompt);
}

void	exec_command(t_px *px, t_ast *cmd_node)
{
	int		j;
	char	**paths;
	char	*final_path;
	char	**commands;
	char	*file_name;

	commands = commands_extractor(cmd_node);
	builtin_fun(cmd_node, commands, px, TO_EXIT);
	paths = path_extractor();
	if (paths == NULL)
		error_handler("Error: problem envp file path", NULL, 1, NULL);
	j = 0;
	while (paths[j])
	{
		final_path = ft_strjoin_3(paths[j], '/', commands[0]);
		if (access(final_path, F_OK) == 0)
			execve_checker(final_path, commands, paths, px);
		free (final_path);
		j++;
	}
	if (ft_strchr(commands[0], '/') && access(commands[0], F_OK) == 0)
		execve_checker(NULL, commands, paths, px);
	file_name = ft_strdup(cmd_node->data);
	exec_command_free_aux(paths, commands, px);
	error_handler(NULL, file_name, -1, NULL);
}

void	execve_checker(char *f_p, char **comms, char **paths, t_px *px)
{
	t_global	*global;
	char		*file;

	global = global_struct();
	if (f_p != NULL && execve(f_p, comms, global->ev) == -1)
	{
		free(f_p);
		file = ft_strdup(comms[0]);
		free_arrays(comms);
		free_arrays(paths);
		error_handler(NULL, file, -1, px);
	}
	else if (f_p == NULL && execve(comms[0], comms, global->ev) == -1)
	{
		file = ft_strdup(comms[0]);
		free_arrays(comms);
		free_arrays(paths);
		error_handler(NULL, file, -1, px);
	}
}

int	executor_function(t_ast *root_tree)
{
	t_px	*px;
	int		exit_code;

	if (root_tree == NULL)
		return (EXIT_FAILURE);
	px = initialize_px(root_tree);
	if (px->num_commands == 0)
	{
		printf("In here\n");
		exit_code = redirections_setup(px->root_tree, px);
		restore_fd(px);
		free_px(px);
		free_global_struct();
		free_struct_to_free();
		return (exit_code);
	}
	else
	{
		exit_code = executor_aux(px, px->root_tree);
		free_px(px);
		free_struct_to_free();
		return (exit_code);
	}
}
