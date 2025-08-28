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
