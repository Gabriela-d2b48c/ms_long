/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_prompt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabriela <gabriela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 10:57:53 by gabriela          #+#    #+#             */
/*   Updated: 2023/09/28 16:25:40 by gabriela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Look into SHELL in environ array, if found, update it
to our own name */
void	ft_change_prompt(t_ms *ms)
{
	int		i;
	char	**split;
	int		found;

	i = 0;
	found = 0;
	while (environ[i])
	{
		split = ft_split(environ[i], '=');
		if (split == NULL)
			return ;
		if (ft_strncmp(split[0], "SHELL", 6) == 0)
		{
			found = 1;
			free_array(split);
			break ;
		}
		else
			free_array(split);
		i++;
	}
	if (found)
		ft_sync_env(i, "SHELL=^ ⨀ ᴥ ⨀ ^", ms);
}

/* Free allocated memory and update so that ms->env array
and the environ array at given index are synchronized. */
void	ft_sync_env(int i, char *str, t_ms *ms)
{
	char	*str_copy;

	free(ms->env[i]);
	str_copy = ft_strdup(str);
	if (str_copy == NULL)
		return ;
	ms->env[i] = str_copy;
	environ[i] = ms->env[i];
}

/* Iterate through environ array to count its elements and
allocate env memory accordingly. Copy each variable from
environ array to the env array, and close it with NULL */
char	**ft_copy_environ(void)
{
	int		count;
	int		i;
	char	**env;

	count = 0;
	while (environ[count])
		count++;
	env = (char **)malloc((count + 1) * sizeof(char *));
	if (env == NULL)
		return (NULL);
	i = 0;
	while (i < count)
	{
		env[i] = ft_strdup(environ[i]);
		i++;
	}
	env[i] = NULL;
	return (env);
}
