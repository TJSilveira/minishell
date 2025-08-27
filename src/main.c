/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:15:50 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 18:15:51 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("minishell: Cannot be called with arguments\n",
			STDERR_FILENO);
		return (127);
	}
	init_global_struct(envp);
	terminal();
	return (0);
}

void safe_print(const char *msg)
{
    int tty_fd = open("/dev/tty", O_WRONLY);
    if (tty_fd != -1) {
        write(tty_fd, msg, strlen(msg));
        close(tty_fd);
    } else {
        // fallback if no tty
        write(STDERR_FILENO, msg, strlen(msg));
    }
}