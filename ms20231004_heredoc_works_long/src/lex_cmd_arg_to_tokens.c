/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_cmd_arg_to_tokens.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabriela <gabriela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 14:51:20 by gabriela          #+#    #+#             */
/*   Updated: 2023/10/01 10:37:14 by gabriela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Take an array of string tokens (command line) 
and count the number of commands in them
(separated by pipe symbol) Return the number of commands */
int	count_cmd(char **tokens)
{
	int	i;
	int	total_cmds;

	i = 0;
	total_cmds = 0;
	if (tokens == NULL)
		return (total_cmds);
	while (tokens[i])
	{
		total_cmds++;
		while (tokens[i] && ft_strncmp(tokens[i], "|", 2) != 0)
			i++;
		if (tokens[i] && ft_strncmp(tokens[i], "|", 2) == 0)
			i++;
	}
	return (total_cmds);
}

/* Allocate memory for a new command node.
Create a new node and store the copy of the
content in it */
t_cmd	*create_new_cmd_node(char **tokens, int *i, int total_arg)
{
	int		j;
	char	**arg;
	t_cmd	*new_node;

	arg = (char **)malloc(sizeof(char *) * (total_arg + 1));
	j = 0;
	while (tokens[*i] && ft_strncmp(tokens[*i], "|", 2) != 0)
	{
		arg[j] = ft_strdup(tokens[*i]);
		(*i)++;
		j++;
	}
	arg[j] = NULL;
	new_node = (t_cmd *)malloc(sizeof(t_cmd));
	new_node->cmd = ft_strdup(arg[0]);
	new_node->arg = arg;
	new_node->next = NULL;
	return (new_node);
}

/* Count how many arguments (strings)
are there for a single command until it 
reaches either the end of the list or a pipe */
int	count_arg(char **tokens, int i)
{
	int	j;
	int	total_arg;

	total_arg = 0;
	j = i;
	while (tokens[j] && ft_strncmp(tokens[j], "|", 2) != 0)
	{
		total_arg++;
		j++;
	}
	return (total_arg);
}

void	tokenize_cmd_and_args(char **tokens, t_ms *ms)
{
	int		i;
	int		total_arg;
	t_cmd	*last;
	t_cmd	*new_node;
	char	**tokens_without_redirs;
	int		count;
	int		a;
	int		b;

	count = 0;
	a = -1;
	b = 0;
	tokens_without_redirs = array_wo_redirs(tokens, &count, a, b);

	i = 0;
	last = NULL;
	while (tokens_without_redirs[i])
	{
		if (!is_not_redir(tokens_without_redirs[i]))
		{
			i += 2;
			continue ;
		}
		total_arg = count_arg(tokens_without_redirs, i);
		new_node = create_new_cmd_node(tokens_without_redirs, &i, total_arg);
		process_redirs(new_node, tokens);
		if (ms->cmd_head == NULL)
		{
			ms->cmd_head = new_node;
			last = new_node;
		}
		else
		{
			last->next = new_node;
			last = new_node;
		}
		if (tokens_without_redirs[i] && ft_strncmp(tokens_without_redirs[i], "|", 2) == 0)
			i++;
	}
}
