/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabriela <gabriela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 11:53:33 by gabriela          #+#    #+#             */
/*   Updated: 2023/09/24 11:56:35 by gabriela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Free all memory associated with the array. First
free each string, then the array itself */
void	free_array(char **array)
{
	int	i;

	i = 0;
	if (array != NULL)
	{
		while (array[i] != NULL)
		{
			free(array[i]);
			i++;
		}
		free(array);
	}
}

/* Free all memory associated with the linked list.
First free each command, then each argument, 
then the linked list itself	*/
void	free_cmd_list(t_cmd *head)
{
	t_cmd	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp->cmd);
		free_array(tmp->arg);
		free(tmp);
	}
}

/* Free all memory associated with the linked list
and exit (used for cd builtin)*/
void	free_and_exit(int status, t_ms *ms)
{
	free_cmd_list(ms->cmd_head);
	free_array(ms->env);
	exit(status);
}
