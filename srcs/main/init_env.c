/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 15:16:49 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/06 13:14:37 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** Split the env variable string on the first '	=' sign,
** and set the var to the 1st string of our split,
** the 2nd is the value. Making sure that our strings after split exist.
*/

static t_var	*init_envvar(char *env_var)
{
	char		**split_var;
	t_var		*new;

	new = malloc(sizeof(t_var));
	if (!new)
		return (NULL);
	split_var = ft_nsplit(env_var, '=', 1);
	if (split_var[0])
		new->name = ft_strdup(split_var[0]);
	else
		new->name = NULL;
	if (split_var[1])
		new->value = ft_strdup(split_var[1]);
	else
		new->value = NULL;
	free_split(&split_var);
	return (new);
}

/*
** Create a new node & fill it with name + value of the var sent.
*/

static t_list	*lstnew_var(t_var *content)
{
	t_list		*new;

	new = (t_list*)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = malloc(sizeof(t_var));
	if (!new->content)
		return (NULL);
	((t_var *)new->content)->value = ft_strdup(content->value);
	((t_var *)new->content)->name = ft_strdup(content->name);
	new->next = NULL;
	return (new);
}

/*
** Loop to create list of env variables. We init the t_var and then init
** a new element in our list
*/

void			init_lstenv(t_ms *ms, t_list **lst_env, char **env)
{
	t_list		*new;
	t_var		*var;
	int			i;

	i = 0;
	while (env[i] != NULL)
	{
		var = init_envvar(env[i]);
		new = lstnew_var(var);
		ft_lstadd_back(lst_env, new);
		if (!ft_strcmp(var->name, "PWD"))
			ms->pwd = ft_strdup(var->value);
		if (!ft_strcmp(var->name, "OLDPWD"))
			ms->old_pwd = ft_strdup(var->value);
		free(var->name);
		free(var->value);
		free(var);
		i++;
	}
}

char			**lst_to_arr(t_list *env)
{
	char		**arr_env;
	t_list		*tmp;
	t_var		*var;
	int			i;

	tmp = env;
	i = 0;
	arr_env = malloc(sizeof(char *) * (ft_lstsize(tmp) + 1));
	if (!arr_env)
		return (NULL);
	while (tmp)
	{
		var = (t_var *)tmp->content;
		arr_env[i] = ft_strdup(var->name);
		arr_env[i] = strjoin_gnl(arr_env[i], "=");
		arr_env[i] = strjoin_gnl(arr_env[i], var->value);
		tmp = tmp->next;
		i++;
	}
	arr_env[i] = NULL;
	return (arr_env);
}
