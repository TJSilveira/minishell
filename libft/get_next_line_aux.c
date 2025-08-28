/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 10:49:57 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/17 10:49:58 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	get_next_line_clean(int fd, int option, char **buffer)
{
	int	i;

	if (fd == -1)
	{
		i = 0;
		while (i < FD_MAX)
		{
			if (buffer[i])
			{
				free(buffer[i]);
				buffer[i] = NULL;
			}
			i++;
		}
		read_buffer(-1, NULL);
		return (1);
	}
	else if (option == TO_CLEAN && buffer[fd])
	{
		free(buffer[fd]);
		buffer[fd] = NULL;
		return (1);
	}
	return (0);
}
