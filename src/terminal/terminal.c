/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:15:16 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 18:15:19 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	terminal(void)
{
	t_prompt_line	*pl;

	pl = to_prompt_line_struct();
	pl->prompt = ft_strdup("\033[35m$minishell> \033[0m");
	while (1)
	{
		parent_signals();
		terminal_input_option();
		if (check_only_terminal(pl->line))
		{
			free(pl->line);
			continue ;
		}
		else
			add_history(pl->line);
		run_command(pl->line);
	}
}

int	run_command(char *line)
{
	t_lexer		*lexer;

	lexer = init_lexer();
	if (!lexer)
		return (EXIT_FAILURE);
	if (lexer_function(line, lexer) == EXIT_FAILURE)
	{
		free_lexer(lexer);
		return (EXIT_FAILURE);
	}
	free(line);
	if (lexer->first_token == NULL && lexer->count_token == 0)
	{
		free(lexer);
		return (EXIT_SUCCESS);
	}
	run_parser_and_executor(lexer);
	return (EXIT_SUCCESS);
}

void	terminal_input_option(void)
{
	t_prompt_line	*pl;

	pl = to_prompt_line_struct();
	if (isatty(STDIN_FILENO))
	{
		pl->line = readline(pl->prompt);
		pl->input_type = INTERACTIVE_MODE;
		if (pl->line == NULL)
			exit_builtin(NULL);
	}
	else
	{
		pl->line = get_next_line(STDIN_FILENO);
		pl->input_type = NONINTERACTIVE_MODE;
		if (pl->line == NULL)
			exit_builtin(NULL);
		if (pl->line[ft_strlen(pl->line) - 1] == '\n')
			pl->line[ft_strlen(pl->line) - 1] = 0;
	}
}

void	run_parser_and_executor(t_lexer *lexer)
{
	t_parser	*par;
	t_ast		*root_tree;
	t_to_free	*to_free;
	t_global	*global;

	global = global_struct();
	par = init_parser(lexer);
	root_tree = parser_function(par, 0);
	to_free = to_free_struct();
	to_free->par = par;
	to_free->root_tree = root_tree;
	to_free->lexer = lexer;
	if (count_nodes(root_tree) == (lexer->count_token - count_paren(lexer)))
		global->exit_code = executor_function(root_tree);
	else
		global->exit_code = 2;
	free_struct_to_free();
}
