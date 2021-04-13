/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 16:00:10 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/13 14:11:50 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int launch_exec(t_ms *ms, t_cmd *cmd)
{
	int pid;
	int status;

	pid = 0;
	search_prog(ms, cmd);
	if (cmd->ret_value == 0)
	{
		pid = fork();
		if (pid == -1)
			printf("error  msg to display\n");
		else if (pid == 0)
		{
			execve(cmd->content[0], cmd->content, ms->arr_env);
			//	exit(0);
		}
		else
		{
			waitpid(pid, &status, 0);
			kill(pid, SIGTERM);
		}
	}
	else
		error_file(ms, cmd);
	return (0);
}

void		set_pipe(int *pipe_value, t_ms *ms, t_cmd *cmd)
{
	printf("ENTER IN SET_PIPE\n");
	pid_t	pid;
	int		fd[2];

	*pipe_value = 0;
	if(cmd && is_type(cmd, PIPES))
	{
		pipe(fd);
		pid = fork();
		if (pid == 0)// Si le fork fonctionne, on ferme notre out, et on attribue a pipin le stdin qu'on utilise plus tard
		{
			close_fd(fd[1]);
			dup2(fd[0], STDIN);
			ms->pipin = fd[0];
			ms->pid = -1; 
			*pipe_value = 2;
		}  
		else //Si notre fork fail, on ferme l'entrée de notre pipe et on reset notre sortie sur le stdout 
		{
			close_fd(fd[0]);
			dup2(fd[1], STDOUT);
			ms->pipout = fd[1];
			ms->pid = pid;
			*pipe_value = 1;
		}
	}
}

void	launch_cmd(t_ms *ms, t_cmd *cmd)
{
	printf("ENTER IN LAUNCH_CMD\n");
	printf("RECURSIVE VALUE : %d\n", ms->recursive);
	printf("cmd->content in LAUNCH_CMD: %s\n", cmd->content[0]);
	if (ms->recursive == 0)
	{
		printf("RECURSIVE 0 NO EXECUTION\n");
		return;
	}
	//execute exit directement et sort du programe
	if (cmd && !ft_strcmp(cmd->content[0], "exit") && !has_pipe(cmd))
		ft_exit(ms, cmd);
	//exec builtin
	else if (cmd && get_bltn(ms, cmd->content[0]))
	{
		printf("\nWe execute the builtin : %s\n", cmd->content[0]);
		ms->last_ret = launch_bltn(ms, cmd);
	}
	//exec cmd
	else if (cmd)
	{
		printf("\nWe fork and execute the cmd : %s\n", cmd->content[0]);
		ms->last_ret = launch_exec(ms, cmd);
	}
	//reset pipe in/out
	close_fd(ms->pipin);
	close_fd(ms->pipout);
	ms->pipin = -1;
	ms->pipout = -1;
	ms->recursive = 0;
	printf("EXIT LAUNCH_CMD\n\n");
}

void set_redirection(t_ms *ms, t_cmd *cmd)
{
	printf("ENTER IN SET_REDIRECTION\n");
	if(cmd->prev)
	{
		//TO DO REDIRIGER LE IN 
		// 6 = <
		if(cmd->prev->type_link == 6)
		{
			return;
		}
		// on redirige le stdout dans notre fichier si > ou >>
		// 7 = > 
		else if (cmd->prev->type_link == 7)
		{
			close_fd(ms->fdout);
			ms->fdout = open(cmd->content[0], O_CREAT | O_WRONLY | O_APPEND, 0644);
			dup2(ms->fdout, STDOUT);
		}
		// 8 = >>
		else if (cmd->prev->type_link == 8)
		{
			close_fd(ms->fdout);
			ms->fdout = open(cmd->content[0], O_CREAT | O_WRONLY | O_TRUNC, 0644);
			dup2(ms->fdout, STDOUT);
		}
	}
}

int	choose_action(t_ms *ms, t_cmd *cmd)
{
	int pipe;

	printf("\nENTER IN CHOOSE_ACTION\n");
	//PRINT PREV & NEXT INFO
	print_action(cmd);

	set_redirection(ms, cmd);
	set_pipe(&pipe, ms, cmd->prev);
	if (cmd->next && !is_type(cmd->next, END_CMD) && pipe != 1) //si on a une cmd next et que c'est pas une fin, on passe à la cmd après le séparateur
		choose_action(ms, cmd->next->next);

	// PRINT INFO FOR THE NEXT CONDITION
	//print_action_exec_condition(cmd, pipe, ms);

	//après la recursion on se situe sur la derniere commande ou une commande après | ou ;
	if ((is_type(cmd->prev, END_CMD) || is_type(cmd->prev, PIPES) || !cmd->prev) && pipe != 1)
		launch_cmd(ms, cmd);
	return (0);
}
