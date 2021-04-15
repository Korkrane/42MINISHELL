/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:52:12 by clorin            #+#    #+#             */
/*   Updated: 2021/04/13 15:55:23 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int					main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	t_ms			ms;

	ft_bzero(&ms, sizeof(t_ms));
	init_ms(&ms, envp);
	ms.cmd = NULL;
	ms.exit = 1;
	ms.in = dup(STDIN);
	ms.out = dup(STDOUT);
	printf("~~~~~~~ Minishell42 ~~~~~~~\n  by (Bahaas / Clorin)\n           V%.1f:\n", VERSION);
	signal(SIGINT, &sig_);
	g_ms = &ms;
	if (argc == 1)
	{
		while (1)
		{
			ms.exit = get_line(&ms);
			if (ms.line && ms.exit)
			{
				if (!valid_quotes(ms.line, ft_strlen(ms.line)))
					line_to_cmd(&ms, ms.line, ms.cmd);
				else
				{
					ft_putstr_fd("minishell: syntax error with open quotes\n", 2);
					ms.last_ret = 2;
				}
				ft_strdel(&ms.line);
				free_cmd(ms.cmd);
			}
		}
		return (0);
	}
	else
	{
		int		r;
		char	*line;
		int		fd;

		line = NULL;
		fd = open(argv[2], O_RDONLY);
		while ((r = get_next_line(fd, &line)) > 0)
		{
			ms.line = ft_strdup(line);
			line_to_cmd(&ms, ms.line, ms.cmd);
			ft_strdel(&ms.line);
			free_cmd(ms.cmd);
			free(line);
			line = NULL;
		}
		printf("%s", line);
		free(line);
		line = NULL;
	}
}
