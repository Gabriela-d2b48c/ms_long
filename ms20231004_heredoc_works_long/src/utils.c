/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabriela <gabriela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 13:04:12 by gabriela          #+#    #+#             */
/*   Updated: 2023/09/29 11:14:04 by gabriela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Return 1 if the command is recognized as 
a builtin, 0 otherwise */
int	ft_check_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "cd", 3) == 0
		|| ft_strncmp(cmd, "echo", 5) == 0
		|| ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0
		|| ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "pwd", 4) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	return (0);
}

/* Loop through the linked list and count the number
of command nodes in it. Return the count */
int	ft_get_command_count(t_cmd *cmd_head)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = cmd_head;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/* Print an error message and exit the program with
status 127 (command not found) */
void	ft_command_not_found(t_cmd *current)
{
	write(2, "Command not found: ", 19);
	write(2, current->cmd, ft_strlen(current->cmd));
	write(2, "\n", 1);
	exit(127);
}
