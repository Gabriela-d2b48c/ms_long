/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_redirections.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabriela <gabriela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 09:13:58 by gabriela          #+#    #+#             */
/*   Updated: 2023/10/04 13:18:45 by gabriela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Checks if a token is not a redirection operator.
Return 1 if the token is not a redirection, 0 otherwise. */
int	is_not_redir(char *tokens)
{
	if ((ft_strncmp(tokens, "<", 2) != 0)
		&& (ft_strncmp(tokens, ">", 2) != 0)
		&& (ft_strncmp(tokens, "<<", 3) != 0)
		&& (ft_strncmp(tokens, ">>", 3) != 0))
		return (1);
	return (0);
}

char	**array_wo_redirs(char **old_array, int *count, int i, int j)
{
	char	**new_array;

	while (old_array[++i] != NULL)
	{
		if (is_not_redir(old_array[i]) == 1)
			(*count)++;
		else
			i++;
	}
	new_array = (char **)malloc(sizeof(char *) * (*count + 1));
	i = -1;
	while (old_array[++i] != NULL)
	{
		if (is_not_redir(old_array[i]) == 1)
		{
			new_array[j] = ft_strdup(old_array[i]);
			j++;
		}
		else
			i++;
	}
	new_array[j] = NULL;
	return (new_array);
}

void	process_redirs(t_cmd *current_cmd, char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (ft_strncmp(tokens[i], "<<", 3) == 0 && tokens[i + 1])
		{
			current_cmd->here_doc = ft_strdup(tokens[i + 1]);
			i += 2;
		}
		else if (ft_strncmp(tokens[i], "<", 2) == 0 && tokens[i + 1])
		{
			current_cmd->input_redir = ft_strdup(tokens[i + 1]);
			i += 2;
		}
		else if (ft_strncmp(tokens[i], ">>", 3) == 0 && tokens[i + 1])
		{
			current_cmd->append_output_redir = ft_strdup(tokens[i + 1]);
			i += 2;
		}
		else if (ft_strncmp(tokens[i], ">", 2) == 0 && tokens[i + 1])
		{
			current_cmd->output_redir = ft_strdup(tokens[i + 1]);
			i += 2;
		}
		else
			i++;
	}
}

void	ft_heredoc(char *delimiter, int heredoc_fd[2])
{
	char	*line;

	pipe(heredoc_fd);
	while (1)
	{
		write(1, "heredoc> ", 9);
		line = readline(NULL);
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(heredoc_fd[1], line, strlen(line));
		write(heredoc_fd[1], "\n", 1);
		free(line);
	}
	close(heredoc_fd[1]);
}