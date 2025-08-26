/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_aux.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:50:11 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 16:50:12 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	qs_swap_str(char **arr_a, char **arr_b)
{
	char	*temp;

	temp = *arr_a;
	*arr_a = *arr_b;
	*arr_b = temp;
}

int	qs_partition(char **arr, int low, int high)
{
	char	*pivot;
	int		i;
	int		j;

	i = low - 1;
	j = low;
	pivot = arr[high];
	while (j < high)
	{
		if (ft_strncmp(arr[j], pivot, ft_strlen(arr[j])) <= 0)
		{
			i++;
			qs_swap_str(&arr[i], &arr[j]);
		}
		j++;
	}
	qs_swap_str(&arr[i + 1], &arr[high]);
	return (i + 1);
}

void	quicksort_strings(char **arr, int low, int high)
{
	int	pivot_index;

	if (low < high)
	{
		pivot_index = qs_partition(arr, low, high);
		quicksort_strings(arr, low, pivot_index - 1);
		quicksort_strings(arr, pivot_index + 1, high);
	}
}

void	print_with_dquotes(char *str)
{
	char	*temp;
	int		i;

	if (ft_strchr(str, '=') == NULL)
		ft_putstr_fd(str, STDOUT_FILENO);
	else
	{
		temp = (char *)malloc(sizeof(char) * (ft_strlen(str) + 3));
		i = -1;
		while (str[++i] != '=')
			temp[i] = str[i];
		temp[i] = str[i];
		i++;
		temp[i++] = '\"';
		while (str[i - 1])
		{
			temp[i] = str[i - 1];
			i++;
		}
		temp[i++] = '\"';
		temp[i++] = 0;
		ft_putstr_fd(temp, STDOUT_FILENO);
		free(temp);
	}
}
