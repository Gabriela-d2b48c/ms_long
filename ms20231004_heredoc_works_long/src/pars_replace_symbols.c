/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_replace_symbols.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabriela <gabriela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 12:20:50 by gabriela          #+#    #+#             */
/*   Updated: 2023/09/29 15:44:06 by gabriela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Transform the input string, so that the spaces are replaced with the token
delimiter to make the input string easier to parse. If it encounters ", ', or {,
it looks for the corresponding closing delimiter and skips everything in between.
*/
char	*ft_transform_for_lexer(const char *in_str)
{
	int		i;
	char	*out_str;
	char	closing_delimiter;

	i = 0;
	out_str = strdup(in_str);
	if (!out_str)
		return (NULL);
	while (out_str[i] != '\0')
	{
		if (out_str[i] == ' ')
			out_str[i] = TOKEN_DELIMITER;
		else if (out_str[i] == '"' || out_str[i] == '\'' || out_str[i] == '{')
		{
			if (out_str[i] == '{')
				closing_delimiter = '}';
			else
				closing_delimiter = out_str[i];
			i++;
			while (out_str[i] != closing_delimiter)
				i++;
		}
		i++;
	}
	return (out_str);
}

/* Traverse each command and its arguments in the linked list, replacing
special symbols with exit status and home directory */
void	ft_replace_symbols(t_ms *ms)
{
	t_cmd	*current;
	int		i;

	current = ms->cmd_head;
	while (current != NULL)
	{
		if (ft_strchr(current->cmd, '~'))
			current->cmd = ft_replace_home_path
				(current->cmd, 0, getenv("HOME"));
		i = 0;
		while (current->arg && current->arg[i] != NULL)
		{
			if (ft_strchr(current->arg[i], '~'))
				current->arg[i] = ft_replace_home_path
					(current->arg[i], 0, getenv("HOME"));
			i++;
		}
		current = current->next;
	}
}

/* Replaces ~ according to the character that follows it.
If it is followed by a /, the tilde is replaced with home_path.
If it is the last character, entire token is replaced with home_path.*/
char	*ft_replace_home_path(char *token, int i, char *home_path)
{
	char	*trimmed;
	char	*temp;

	while (token[i] != '\0')
	{
		if (token[i] == '~')
		{
			if (token[i + 1] == '/')
			{
				trimmed = ft_strtrim(token, "~");
				temp = ft_strjoin(home_path, trimmed);
				free(trimmed);
				free(token);
				return (temp);
			}
			else if (token[i + 1] == '\0')
			{
				free(token);
				return (ft_strdup(home_path));
			}
			break ;
		}
		i++;
	}
	return (token);
}

void	ft_replace_exit_status(t_cmd *cmd_head, int i, int exit_status)
{
	t_cmd	*current;
	char	*str_exit_status;

	str_exit_status = ft_itoa(exit_status >> 8);
	current = cmd_head;
	while (current)
	{
		if (ft_strncmp(current->cmd, "$?", 3) == 0)
		{
			free(current->cmd);
			current->cmd = strdup(str_exit_status);
		}
		i = 0;
		while (current->arg && current->arg[i])
		{
			if (ft_strncmp(current->arg[i], "$?", 3) == 0)
			{
				free(current->arg[i]);
				current->arg[i] = strdup(str_exit_status);
			}
			i++;
		}
		current = current->next;
	}
	free(str_exit_status);
}
