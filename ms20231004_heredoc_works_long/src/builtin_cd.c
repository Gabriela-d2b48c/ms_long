/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabriela <gabriela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 11:36:40 by gabriela          #+#    #+#             */
/*   Updated: 2023/09/24 12:30:55 by gabriela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Count the number of arguments in the command. */
int	ft_command_counting(t_cmd *current)
{
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (current->arg[i])
	{
		count++;
		i++;
	}
	return (count);
}

/* Change the current directory to the directory
specified in dest. Update PWD and OLDPWD. */
int	ft_move_dir(char *dest, char *old_pwd, char *cwd, t_ms *ms)
{
	if (chdir(dest) == -1)
		return (perror("chdir"), 256);
	else
	{
		getcwd(cwd, 1048);
// Erase this printf later
		printf("Changed directory to: %s\n", cwd);
		ft_update_or_add_env("PWD", cwd, ms);
		ft_update_or_add_env("OLDPWD", old_pwd, ms);
	}
	return (0);
}

/* To be used if cd is part of a pipeline, when
it's executed in a forked process */
void	ft_cd_as_forked(t_cmd *current, t_ms *ms)
{
	int		argument_count;
	char	old_pwd[1048];
	char	cwd[1048];
	int		status;

	status = 0;
	argument_count = ft_command_counting(current);
	getcwd(old_pwd, 1048);
	if (argument_count > 2)
	{
		printf("cd: too many arguments\n");
		free_and_exit(1, ms);
	}
	else if (argument_count == 1 && getenv("HOME"))
		status = ft_move_dir(getenv("HOME"), old_pwd, cwd, ms);
	else if (argument_count > 1 && strcmp(current->arg[1], "-") == 0
		&& getenv("OLDPWD"))
		status = ft_move_dir(getenv("OLDPWD"), old_pwd, cwd, ms);
	else if (argument_count > 1)
	{
		status = ft_move_dir(current->arg[1], old_pwd, cwd, ms);
		if (status == 256)
			printf("cd: %s: no such file or directory.\n", current->arg[1]);
	}
	free_and_exit(0, ms);
}

/* To be used if cd is not part of a pipeline and
is executed directly */
int	ft_cd_directly(t_cmd *current, t_ms *ms)
{
	int		argument_count;
	char	old_pwd[1048];
	char	cwd[1048];
	int		status;

	status = 0;
	argument_count = ft_command_counting(current);
	getcwd(old_pwd, 1048);
	if (argument_count > 2)
		return (printf("cd: too many arguments\n"), 256);
	else if (argument_count == 1 && getenv("HOME"))
		status = ft_move_dir(getenv("HOME"), old_pwd, cwd, ms);
	else if (argument_count > 1 && strcmp(current->arg[1], "-") == 0
		&& getenv("OLDPWD"))
		status = ft_move_dir(getenv("OLDPWD"), old_pwd, cwd, ms);
	else if (argument_count > 1)
	{
		status = ft_move_dir(current->arg[1], old_pwd, cwd, ms);
		if (status == 256)
			printf("cd: %s: no such file or directory.\n", current->arg[1]);
	}
	return (status);
}

/* search for an existing environment variable,
update it if found, or add a new one if not found,
free memory.*/
void	ft_update_or_add_env(char *var, char *value, t_ms *ms)
{
	int		i;
	char	*new_env_str;
	char	**split;
	size_t	len;

	i = 0;
	while (ms->env[i])
	{
		split = ft_split(ms->env[i], '=');
		if (strcmp(split[0], var) == 0)
		{
			free_array(split);
			break ;
		}
		free_array(split);
		i++;
	}
	len = strlen(var) + strlen(value) + 2;
	new_env_str = (char *)malloc(len);
	ft_strlcpy(new_env_str, var, len);
	ft_strlcat(new_env_str, "=", len);
	ft_strlcat(new_env_str, value, len);
	if (ms->env[i])
		ft_sync_env(i, new_env_str, ms);
	free(new_env_str);
}
