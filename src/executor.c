#include "../includes/minishell.h"

/* Start of Redirections */

int	open_fd(char *path, int option, t_px *px)
{
	int	fd;

	fd = -1;
	if (option == CHAR_INRED)
		fd = open(path, O_RDONLY);
	else if (option == CHAR_OUTRED)
		fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	else if (option == CHAR_HEREDOC)
		fd = heredoc(path, px);
	else if (option == CHAR_APPEND)
		fd = open(path, O_WRONLY | O_APPEND | O_CREAT, 0777);
	if (fd == -1 && CHAR_INRED == 'I')
		perror("Error: opening file");
	return (fd);
}

void	write_line_break(int fd, t_px *px, char *line, char *limitor)
{
	size_t			size;
	t_prompt_line	*pl;

	pl = to_prompt_line_struct();
	size = ft_strlen(limitor);
	if (size == ft_strlen(line) && ft_strncmp(limitor, line, size) == 0)
	{
		free(line);
		free(limitor);
		close(fd);
		get_next_line(-1);
		free_px(px);
		free_global_struct();
		free_struct_to_free();
		free(pl->prompt);
		exit(EXIT_SUCCESS);
	}
}

int	write_line(char *limit, int fd, t_px *px)
{
	char			*line;
	char			*limitor;

	limitor = ft_strjoin(limit, "\n");
	while (1)
	{
		write(px->fd_stdout, "> ", 2);
		line = get_next_line(0);
		write_line_break(fd, px, line, limitor);
		if (write(fd, line, ft_strlen(line)) == -1)
			error_handler("Writing lines", NULL, 1, NULL);
		free(line);
	}
	exit(EXIT_FAILURE);
}

int	heredoc(char *limit, t_px *px)
{
	int	pipe_fd[2];
	int	pid;

	if (pipe(pipe_fd) == -1)
		error_handler("Laying down the pipe(s)", NULL, 1, NULL);
	pid = fork();
	if (pid == -1)
		error_handler("Fork creation", NULL, 1, NULL);
	if (pid == 0)
	{
		close(pipe_fd[READ]);
		write_line(limit, pipe_fd[WRITE], px);
	}
	else
	{
		close(pipe_fd[WRITE]);
		waitpid(pid, NULL, 0);
		return (pipe_fd[READ]);
	}
	return (-1);
}

void	redirections_files_setup(int fd, int type)
{
	if (fd == -1)
		return ;
	if (type == CHAR_INRED || type == CHAR_HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			error_handler("Duplicating read-end pipe to STDIN", NULL, 1, NULL);
		close(fd);
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			error_handler("Duplicating write-end pipe to STDOUT",
				NULL, 1, NULL);
		close(fd);
	}
}

int	redirections_setup(t_ast *root, t_px *px)
{
	int	fd;

	while (root)
	{
		if (is_redirect_token(root->type))
		{
			fd = open_fd(root->right->data, root->type, px);
			if (errno == EACCES || fd == -1)
			{
				restore_fd(px);
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(root->right->data, STDERR_FILENO);
				ft_putstr_fd(": ", STDERR_FILENO);
				ft_putstr_fd(strerror(errno), STDERR_FILENO);
				ft_putstr_fd("\n", STDERR_FILENO);
				return (EXIT_FAILURE);
			}
			redirections_files_setup(fd, root->type);
		}
		root = root->right;
	}
	return (EXIT_SUCCESS);
}

/* End of Redirections */

/* Start of Initializations */

int	count_number_commands(t_ast *root_tree)
{
	int	total;

	total = 0;
	if (root_tree == NULL)
		return (0);
	if (is_default_token(root_tree->type))
		total++;
	else if (is_operator_token(root_tree->type))
	{
		total += count_number_commands(root_tree->left);
		total += count_number_commands(root_tree->right);
	}
	return (total);
}

t_px	*initialize_px(t_ast *root_tree)
{
	t_px	*px;

	px = malloc(sizeof(t_px));
	malloc_error_handler(px, EXIT_FAILURE);
	px->num_commands = count_number_commands(root_tree);
	px->root_tree = root_tree;
	px->fd_stdin = dup(STDIN_FILENO);
	px->fd_stdout = dup(STDOUT_FILENO);
	return (px);
}

/* End of Initializations*/

/* Start of Executor functions */

/* executes a subtree */
int	executor_aux(t_px *px, t_ast *root)
{
	int	exit_code;

	if (root == NULL)
		return (EXIT_SUCCESS);
	if (is_default_token(root->type))
		return (executor(px, root));
	else if (root->type == CHAR_PIPE)
		return (executor_pipe(px, root));
	else if (root->type == CHAR_AND)
	{
		exit_code = executor_aux(px, root->left);
		if (exit_code != 0)
			return (exit_code);
		return (executor_aux(px, root->right));
	}
	else if (root->type == CHAR_OR)
	{
		exit_code = executor_aux(px, root->left);
		if (exit_code == 0)
			return (exit_code);
		return (executor_aux(px, root->right));
	}
	return (EXIT_SUCCESS);
}

void	restore_fd(t_px *px)
{
	dup2(px->fd_stdin, STDIN_FILENO);
	dup2(px->fd_stdout, STDOUT_FILENO);
	close(px->fd_stdin);
	close(px->fd_stdout);
}

int	executor_pipe_left(t_px *px, t_ast *root, int pipe_fd[2])
{
	int	exit_code;

	child_signals();
	dup2(pipe_fd[WRITE], STDOUT_FILENO);
	close(pipe_fd[READ]);
	exit_code = execute_subshell(px, root->left);
	free_struct_to_free();
	free_global_struct();
	exit (exit_code);
}

int	executor_pipe_right(t_px *px, t_ast *root, int pipe_fd[2])
{
	int	exit_code;

	child_signals();
	dup2(pipe_fd[READ], STDIN_FILENO);
	close(pipe_fd[WRITE]);
	exit_code = execute_subshell(px, root->right);
	free_struct_to_free();
	free_global_struct();
	exit (exit_code);
}

int	executor_pipe_return(int pipe_fd[2], int pids[2], int *status)
{
	close(pipe_fd[READ]);
	close(pipe_fd[WRITE]);
	waitpid(pids[0], status, 0);
	if (WIFSIGNALED(*status))
		write(1, "\n", 1);
	waitpid(pids[1], status, 0);
	if (WIFEXITED(*status))
		return (WEXITSTATUS(*status));
	else if (WIFSIGNALED(*status))
	{
		if (WTERMSIG(*status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(*status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		return (WTERMSIG(*status) + 128);
	}
	return (EXIT_FAILURE);
}

int	executor_pipe(t_px *px, t_ast *root)
{
	int	pipe_fd[2];
	int	pids[2];
	int	status;

	status = 0;
	if (pipe(pipe_fd) != 0)
		error_handler("Laying down the pipe(s)", NULL, 1, NULL);
	pids[0] = fork();
	if (pids[0] == 0)
		executor_pipe_left(px, root, pipe_fd);
	ignore_signals();
	pids[1] = fork();
	if (pids[1] == 0)
		executor_pipe_right(px, root, pipe_fd);
	return (executor_pipe_return(pipe_fd, pids, &status));
}

int	execute_subshell(t_px *px, t_ast *root)
{
	t_px			*px_subshell;
	int				exit_code;
	t_prompt_line	*pl;

	pl = to_prompt_line_struct();
	px_subshell = initialize_px(root);
	px_subshell->fd_stdin = px->fd_stdin;
	px_subshell->fd_stdout = px->fd_stdout;
	exit_code = executor_aux(px_subshell, root);
	free_px(px);
	free_px(px_subshell);
	free(pl->prompt);
	return (exit_code);
}

int	executor_return(int *status, int pid)
{
	ignore_signals();
	waitpid(pid, status, 0);
	if (WIFEXITED(*status))
		return (WEXITSTATUS(*status));
	else if (WIFSIGNALED(*status))
	{
		if (WTERMSIG(*status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(*status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		return (WTERMSIG(*status) + 128);
	}
	return (EXIT_FAILURE);
}

int	executor(t_px *px, t_ast *cmd_node)
{
	int	status;
	int	pid;

	if (is_builtin(px->root_tree))
		return (executor_builtin_func(px));
	pid = fork();
	status = 0;
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		child_signals();
		status = redirections_setup(cmd_node, px);
		if (status == EXIT_FAILURE)
			exit (EXIT_FAILURE);
		if (cmd_node->data == NULL || cmd_node->data[0] == 0)
			error_handler("No command ''", NULL, 1, px);
		if (is_default_token(cmd_node->type))
			exec_command(px, cmd_node);
	}
	return (executor_return(&status, pid));
}

/* Executes a subtree that is only composed out of a builtin function. */
int	executor_builtin_func(t_px *px)
{
	int	exit_code;

	exit_code = redirections_setup(px->root_tree, px);
	if (exit_code == EXIT_SUCCESS)
		exit_code = builtin_functions(px->root_tree, NULL, px, TO_RETURN);
	dup2(px->fd_stdin, STDIN_FILENO);
	dup2(px->fd_stdout, STDOUT_FILENO);
	close(px->fd_stdin);
	close(px->fd_stdout);
	return (exit_code);
}

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
	builtin_functions(cmd_node, commands, px, TO_EXIT);
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

void	execve_checker(char *f_path, char **comms, char **paths, t_px *px)
{
	t_global	*global;
	char		*file;

	global = global_struct();
	if (f_path != NULL && execve(f_path, comms, global->ev) == -1)
	{
		free(f_path);
		file = ft_strdup(comms[0]);
		free_arrays(comms);
		free_arrays(paths);
		error_handler(NULL, file, -1, px);
	}
	else if (f_path == NULL && execve(comms[0], comms, global->ev) == -1)
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

/* End of Executor Functions */

/* Executor AUX functions */

void	error_handler_efault(char *file_name)
{
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (file_name)
		free(file_name);
	exit(126);
}

void	error_handler_eacces(char *file_name)
{
	struct stat	file_stat;

	if (stat(file_name, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
	{
		ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		free(file_name);
		exit(126);
	}
	else
	{
		ft_putstr_fd("Permission denied\n", STDERR_FILENO);
		free(file_name);
		exit(126);
	}
}

void	error_handler_enoent(char *file_name)
{
	if (ft_strchr(file_name, '/'))
		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
	else
		ft_putstr_fd("command not found\n", STDERR_FILENO);
	free(file_name);
	exit(127);
}

void	error_handler(char *msg, char *file_name, int error_code, t_px *px)
{
	if (px)
		free_px(px);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (file_name)
	{
		ft_putstr_fd(file_name, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (msg)
	{
		ft_putstr_fd(msg, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	if (error_code != -1)
		exit(error_code);
	if (errno == EFAULT)
		error_handler_EFAULT(file_name);
	else if (errno == EACCES)
		error_handler_EACCES(file_name);
	else if (errno == ENOENT && file_name)
		error_handler_ENOENT(file_name);
}

void	malloc_error_handler(void *ptr, int error_code)
{
	if (!ptr)
		error_handler("Error with malloc", NULL, error_code, NULL);
}

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
	while (global->ev[++i])
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

void	free_arrays(char **arrays)
{
	int	i;

	i = 0;
	while (arrays[i])
	{
		free(arrays[i]);
		i++;
	}
	free(arrays);
}

void	free_px(t_px *px)
{
	if (px)
	{
		if (px->fd_stdin > 2)
			close(px->fd_stdin);
		if (px->fd_stdout > 2)
			close(px->fd_stdout);
		free(px);
		px = NULL;
	}
}

void	free_struct_to_free(void)
{
	t_to_free	*to_free;

	to_free = to_free_struct();
	if (to_free->lexer)
	{
		free_lexer(to_free->lexer);
		to_free->lexer = NULL;
	}
	if (to_free->root_tree)
	{
		free_ast(to_free->root_tree);
		to_free->root_tree = NULL;
	}
	if (to_free->par)
	{
		free_parser_struct(to_free->par);
		to_free->par = NULL;
	}
}

/* END of Executor AUX functions*/