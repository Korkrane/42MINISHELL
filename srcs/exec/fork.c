/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 14:55:03 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/04 12:10:33 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void		free_end_exit(t_ms *ms, int ret)
{
	free_exit(ms);
	exit(ret);
}

void			select_execution(t_ms *ms, t_cmd *cmd, int exit_in_pipeline)
{
	if (cmd && get_bltn(ms, cmd->content[0]))
	{
		ms->last_ret = launch_bltn(ms, cmd);
		if (exit_in_pipeline)
			free_end_exit(ms, ms->last_ret);
	}
	else
		search_prog(ms, cmd);
}

void			child_execution(t_ms *ms, t_cmd **cmd, int fdd, int *fd)
{
	dup2(fdd, STDIN);
	if ((*cmd)->next && (*cmd)->next->type_link == 4)
		dup2(fd[1], STDOUT);
	if ((*cmd)->next && is_redir((*cmd)->next))
		set_redirection(ms, (*cmd));
	if (ms->ret)
		free_end_exit(ms, ms->ret);
	close(fd[0]);
	if (ft_is_empty((*cmd)->content[0]) && !(*cmd)->is_env)
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		free_end_exit(ms, 127);
	}
	if (ft_is_empty((*cmd)->content[0]) && (*cmd)->is_env)
		free_end_exit(ms, 0);
	select_execution(ms, (*cmd), 1);
	if ((*cmd)->ret_value)
		free_end_exit(ms, ms->last_ret);
	free_arrstr(ms->arr_env);
	ms->arr_env = lst_to_arr(ms->env);
	execve((*cmd)->content[0], (*cmd)->content, ms->arr_env);
}

void			parent_execution(int *fdd, int *fd)
{
	close(fd[1]);
	*fdd = fd[0];
}

void			fork_error(t_ms *ms)
{
	ft_putstr_fd("minishell: fork: Out of memory\n", 2);
	free_end_exit(ms, 12);
}
