/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42luxembourg.l  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 14:13:10 by tsilveir          #+#    #+#             */
/*   Updated: 2025/08/18 14:13:27 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_global	*global_struct(void)
{
	static t_global	global;

	return (&global);
}

t_to_free	*to_free_struct(void)
{
	static t_to_free	to_free;

	return (&to_free);
}

t_prompt_line	*to_prompt_line_struct(void)
{
	static t_prompt_line	pl;

	return (&pl);
}
