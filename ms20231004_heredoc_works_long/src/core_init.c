/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabriela <gabriela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 15:06:35 by gabriela          #+#    #+#             */
/*   Updated: 2023/09/16 12:06:37 by gabriela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Initialize the state for processing commands */
void	ft_init(t_ms *ms)
{
	ms->cmd_head = NULL;
	ms->i_last_cmd = -1;
	ms->in_fd = -1;
	ms->out_fd = -1;
	ms->delim = NULL;
	ms->saved_stdin = -1;
	ms->saved_stdout = -1;
	ms->exit_status = 0;
	ms->tokens = NULL;
	ms->env = ft_copy_environ();
	environ = ms->env;
	ft_change_prompt(ms);
}

/* Reset the state for processing commands changed
during the command execution */
void	ft_reset(t_ms *ms)
{
	ms->cmd_head = NULL;
	ms->i_last_cmd = -1;
	ms->delim = NULL;
	ms->in_fd = -1;
	ms->out_fd = -1;
	ms->saved_stdin = -1;
	ms->saved_stdout = -1;
	ms->tokens = NULL;
}
