/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_cmd_type.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabriela <gabriela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 10:37:19 by gabriela          #+#    #+#             */
/*   Updated: 2023/09/08 08:43:23 by gabriela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*  Iterates through the list of commands and checks
what type of command each one is. If it is a builtin 
or directly executable cmd located at specified path,
go forward (no action is needed). 
If not, find and construct the path */
void	ft_check_cmd_type(t_ms *ms)
{
	t_cmd	*current;

	current = ms->cmd_head;
	while (current)
	{
		if ((ft_check_builtin(current->cmd) == 1)
			|| (access(current->cmd, X_OK) == 0))
		{
			current = current->next;
			continue ;
		}
		else if (ft_check_path_exec(current->cmd) == 1)
		{
			current->cmd = ft_return_path(current->cmd);
			current = current->next;
			continue ;
		}
		else
			break ;
	}
}

/* Check if the given command exists in the system.
Returns 1 if the command is found and executable. */
int	ft_check_path_exec(char *cmd)
{
	int		i;
	char	**paths;
	int		result;

	i = 0;
	while (environ[i] != NULL)
	{
		if (ft_strnstr(environ[i], "PATH", 4) != 0)
			break ;
		i++;
	}
	if (environ[i] == NULL)
		return (0);
	paths = ft_split(environ[i] + 5, ':');
	result = ft_search_for_exec(paths, cmd);
	free_array(paths);
	return (result);
}

/* Searches for an executable command in paths.
 Returns 1 if the command is found and executable */
int	ft_search_for_exec(char **paths, char *cmd)
{
	int		i;
	int		path_length;
	int		cmd_length;
	char	*path;

	i = 0;
	cmd_length = ft_strlen(cmd);
	while (paths[i] != NULL)
	{
		path_length = ft_strlen(paths[i]);
		path = malloc(path_length + cmd_length + 2);
		if (path == NULL)
			return (0);
		ft_strlcpy(path, paths[i], path_length + 1);
		path[path_length] = '/';
		ft_strlcpy(path + path_length + 1, cmd, cmd_length + 1);
		if (access(path, X_OK) == 0)
		{
			free(path);
			return (1);
		}
		free(path);
		i++;
	}
	return (0);
}

/* The other half of ft_return_path, which would be
otherwise too long */
char	*ft_search_and_get_path(char **paths, char *cmd, int i)
{
	int		cmd_length;
	char	*new_cmd;
	char	*path;
	int		path_length;

	cmd_length = ft_strlen(cmd);
	new_cmd = NULL;
	while (paths[i])
	{
		path_length = ft_strlen(paths[i]);
		path = malloc(path_length + cmd_length + 2);
		if (path == NULL)
			return (NULL);
		ft_strlcpy(path, paths[i], path_length + 1);
		path[path_length] = '/';
		ft_strlcpy(path + path_length + 1, cmd, cmd_length + 1);
		if (access(path, X_OK) == 0)
		{
			new_cmd = path;
			break ;
		}
		free(path);
		i++;
	}
	return (new_cmd);
}

/* Constructs and returns full path for an executable command
by searching through the given paths. Returns NULL if the command
is not found, ortherwise returns the full path to the command */
char	*ft_return_path(char *cmd)
{
	int		i;
	char	**paths;
	char	*new_cmd;

	i = 0;
	new_cmd = NULL;
	while (environ[i] != NULL)
	{
		if (ft_strnstr(environ[i], "PATH", 4) != 0)
			break ;
		i++;
	}
	if (environ[i] == NULL)
		return (NULL);
	paths = ft_split(environ[i] + 5, ':');
	new_cmd = ft_search_and_get_path(paths, cmd, 0);
	free_array(paths);
	if (new_cmd != NULL)
		free(cmd);
	return (new_cmd);
}
