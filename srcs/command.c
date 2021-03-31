/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 14:52:26 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/31 15:52:47 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd		*create_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;
	t_cmd	*new;

	tmp = *cmd;
	new = ft_memalloc(sizeof(t_cmd));
	if (new == NULL)
		return (NULL);
	while (tmp != NULL && tmp->next != NULL)
		tmp = tmp->next;
	if (tmp == NULL)
		*cmd = new;
	else
		tmp->next = new;
	return (new);
}

t_cmd	*tokens_to_cmd(t_ms *ms, t_cmd **cmd, t_tokens **tokens)
{
	t_cmd *new_cmd;
	t_tokens *count;
	t_tokens *head;

	new_cmd = malloc(sizeof(t_cmd));
	int i = 0;
	head = *tokens;
	count = *tokens;
	while(count)
	{
		if(count->type_content == CMD || count->type_content == ARGS || count->type_content == REDIR)
			i++;
		else
			break;
		count = count->next;
	}
	printf("test i : %d\n", i);
	new_cmd->content = malloc(sizeof(char **));
	new_cmd->next = NULL;
	int j = 0;
	while(j < i)
	{
		//printf("tokens->content : %s\n", head->content);
		new_cmd->content[j] = ft_strdup(head->content);
		printf("new_cmd->content[%d] : %s\n", j, new_cmd->content[j]);
		head = head->next;
		j++;
	}
	printf("test j : %d\n", j);
	new_cmd->content[j] = NULL;
	return (new_cmd);
}

void	tmp_line_to_cmd(t_ms *ms, char *line)
{
	t_tokens *tokens;
	int ret;
	t_cmd *cmd;

	tokens = NULL;
	ret = 0;
	printf("(%d)line = %s\n", ms->exit, ms->line);
	if(ret == 0)
		ret = get_tokens(ms, &tokens, line);
	int i = 0;
	printf("type_content : 0 = CMD, 1 = ARGS, 2 = PIPES, 3 = REDIR, 4 = END_CMD \n");
	printf("type_quotes : 0 = NO_QUOTES, 34 = DQUOTE, QUOTE = 39\n");
	printf("TOKENS =  \n");
	t_tokens *a;
	a = tokens;
	while(a)
	{
		printf("|%s|  |type_quote : %d| |type_content : %d|\n", a->content, a->type_quote, a->type_content);
		/*
		if(a->prev)
			printf("|previous token type_content : %d|\n", a->prev->type_content);
		else
			printf("|previous token doesn't exist|\n");*/
		a = a->next;
	}
	printf("COMMAND = \n");
	cmd = tokens_to_cmd(ms, &cmd, &tokens);
	int k;
	int cmd_nb = 0;
	t_cmd *tmp = cmd;
	while(tmp)
	{
		k = 0;
		printf("cmd_nb : %d\n", cmd_nb);
		int count = 0;
		while(tmp->content[k])
		{
			printf("cmd->content[%d] : %s\n", k, tmp->content[k]);
			k++;
		}
		cmd_nb++;
		tmp = tmp->next;
	}
	execute(ms, cmd);
/*	
	cmd = malloc(sizeof(t_cmd));
	// cmd->content = ft_split(line, ' ');
	cmd->content = parse(line);
	int i = 0;
		printf("cmd = ");
	while(cmd->content[i])
		printf("<%s> ", cmd->content[i++]);
	printf("\n");
	cmd->next = NULL;
	cmd->ret_value = 0;
	execute(ms, cmd);
*/	
	//CHECK UPDATE PWD & OLDPWD with cd use
	/*
	printf("ms.pwd = %s\n", ms->pwd);
	printf("ms.oldpwd = %s\n", ms->old_pwd);
	char *pwd_env = ft_getenv(&ms->env, "PWD", 1);
	char *oldpwd_env = ft_getenv(&ms->env, "OLDPWD", 1);
	printf("ms->env->pwd = %s\n", pwd_env);
	printf("ms->env->oldpwd = %s\n", oldpwd_env);
	*/
}
