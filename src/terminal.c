#include "../includes/minishell.h"

/* TODO: 
==> terminal is wrapping aroung and going over the prompt
==> test all the builtin functions with the env variables unset */

int	count_nodes(t_ast *root_tree)
{
	int	total;

	total = 0;
	if (root_tree == NULL)
		return (0);
	total++;
	total += count_nodes(root_tree->left);
	total += count_nodes(root_tree->right);
	return (total);
}

int	count_paren(t_lexer *lexer)
{
	int		total;
	int		i;
	t_token	*curr_tok;

	total = 0;
	i = 0;
	if (lexer == NULL || lexer->first_token == NULL)
		return (0);
	curr_tok = lexer->first_token;
	while (i < lexer->count_token)
	{
		if (curr_tok->type == CHAR_OPAREN || curr_tok->type == CHAR_CPAREN)
			total++;
		curr_tok = curr_tok->next;
		i++;
	}
	return (total);
}

int	run_command(char *line)
{
	t_parser	*par;
	t_ast		*root_tree;
	t_lexer		*lexer;
	t_global	*global;
	t_to_free	*to_free;

	/* inits */
	global = global_struct();
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
		return(EXIT_SUCCESS);
	}
	
	// for (int i = 0; i < lexer->count_token; i++)
	// {
	// 	printf("'%s' ", lexer->first_token->data);
	// 	lexer->first_token = lexer->first_token->next;
	// }
	// exit(0);
		
	par = init_paser(lexer);
	
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
	return (EXIT_SUCCESS);
}

void	terminal()
{
	t_prompt_line	*pl;

	pl = to_prompt_line_struct();
	pl->prompt = ft_strdup("\033[35m$minishell> \033[0m");
	while (1)
	{
		parent_signals();
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
		if (check_only_terminal(pl->line))
		{
			free(pl->line);
			continue;
		}
		else
			add_history(pl->line);
		run_command(pl->line);
	}
}