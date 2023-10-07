/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_env_replace.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabriela <gabriela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 12:20:55 by gabriela          #+#    #+#             */
/*   Updated: 2023/09/23 10:49:47 by gabriela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*  Check quotes for each argument in the command. If quotes are found,
trim them and performs an environment variable replacement for double
quotes. Single quotes are used to preserve the literal value of each
character within the quotes, so variables and special characters do not
get expanded when they are enclosed in single quotes. */
void	handle_quotes(char **arg)
{
	char	*trimmed;
	int		i;

	i = 0;
	while (arg[i])
	{
		if (arg[i][0] == '"')
		{
			arg[i] = ft_replace_env_var(arg[i]);
			trimmed = ft_strtrim(arg[i], "\"");
			free(arg[i]);
			arg[i] = trimmed;
		}
		else if (arg[i][0] == '\'')
		{
			trimmed = ft_strtrim(arg[i], "'");
			free(arg[i]);
			arg[i] = trimmed;
		}
		i++;
	}
}

/* Splitting above function because of stupid norminette */
void	ft_handle_quoted_env_vars(t_ms *ms)
{
	t_cmd	*current_cmd;

	current_cmd = ms->cmd_head;
	while (current_cmd)
	{
		handle_quotes(current_cmd->arg);
		current_cmd = current_cmd->next;
	}
}

/* Preprocessing each command's argument to replace any environment
variables before the command is executed, while avoiding replacing
anything with quoted arguments.*/
void	ft_handle_unquoted_env_vars(t_ms *ms)
{
	t_cmd	*current_cmd;
	int		i;
	int		len;

	current_cmd = ms->cmd_head;
	while (current_cmd)
	{
		i = 0;
		while (current_cmd->arg[i])
		{
			len = ft_strlen(current_cmd->arg[i]);
			if ((current_cmd->arg[i][0] != '\''
				&& current_cmd->arg[i][len - 1] != '\'')
				&& (current_cmd->arg[i][0] != '"'
				&& current_cmd->arg[i][len - 1] != '"'))
				current_cmd->arg[i] = ft_replace_env_var(current_cmd->arg[i]);
			i++;
		}
		current_cmd = current_cmd->next;
	}
}

/* Return the starting index of the next environment variable found
in token, while $ is found. If none is found, return -1. */
int	ft_find_next_env_var(char *token, int start, int *j)
{
	int	i;

	i = start;
	*j = 0;
	while (token[i] != '\0')
	{
		if (token[i] == '$')
		{
			while (ft_isalnum(token[i + 1 + *j])
				|| token[i + 1 + *j] == '_')
				(*j)++;
			if (*j != 0)
			{
				return (i);
			}
		}
		i++;
	}
	return (-1);
}

/* Replace all occurencies of the environment variable found in
a given token, return the updated token with replaced env. var. */
char	*ft_replace_env_var(char *token)
{
	int		i;
	int		j;
	char	*env_var;
	char	*token_update;
	char	*new_str;

	i = 0;
	j = 0;
	while ((i = ft_find_next_env_var(token, i, &j)) != -1)
	{
		token_update = ft_substr(token, i + 1, j);
		env_var = getenv(token_update);
		if (env_var == NULL)
			env_var = "";
		new_str = ft_substr(token, 0, i);
		token_update = ft_strjoin(new_str, env_var);
		free(new_str);
		new_str = ft_strjoin(token_update, token + i + j + 1);
		free(token_update);
		free(token);
		token = new_str;
		i = 0;
		j = 0;
	}
	return (token);
}
