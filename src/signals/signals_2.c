/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:14:55 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 18:14:56 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sigint_handler_heredoc(int signal)
{
	t_global	*global;
	t_px		*px;

	(void)signal;
	px = px_struct_fun(READ, NULL);
	get_next_line(-1, TO_CLEAN);
	global = global_struct();
	write(px->fd_stdout, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	exec_command_free_aux(NULL, NULL);
	free_px_fds(px);
	exit (130);
}

void	heredoc_signals(void)
{
	signal(SIGINT, sigint_handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
}
