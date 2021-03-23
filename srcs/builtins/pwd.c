/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 14:15:54 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/23 16:53:48 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_pwd(t_ms *ms, t_cmd *cmd)
{
	char *pwd;

	if(cmd->content[1])
		printf("pwd: too many arguments\n");
	else
		printf("%s\n", ms->pwd);
	return (0);
}