/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 10:00:01 by clorin            #+#    #+#             */
/*   Updated: 2021/04/13 10:00:16 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sig_(int code)
{
	g_ms->signal = code;
	g_ms->last_ret = 128 + code;
	if (g_ms->pid)
		write(1, "\n", 1);
	else
	{
		write(1,"^C\n", 3);
		prompt(g_ms);
	}
}

void	ctr_c(t_termcaps *tc, t_ms *ms)
{
	ft_strdel(&tc->line);
	tc->cur_pos = 0;
	ms->signal = FALSE;
	get_cursor_position(&tc->start_col, &tc->start_row);
	tc->start_col = 0;
}