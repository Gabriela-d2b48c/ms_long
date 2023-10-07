/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_handle_io.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabriela <gabriela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 10:42:29 by gabriela          #+#    #+#             */
/*   Updated: 2023/09/27 15:13:08 by gabriela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Restore original standard input and output using dup2
and close the saved standard input an output file descriptors */
void	ft_restore_input_output(t_ms *ms)
{
	dup2(ms->saved_stdout, STDOUT_FILENO);
	dup2(ms->saved_stdin, STDIN_FILENO);
	close(ms->saved_stdin);
	close(ms->saved_stdout);
}

/* Redirect the standard input to in_fd using dup2 and
then close it. If delimiter is set, fork a child process,
call heredoc, and exit. Parent waits for the child process
to finish. */
void	ft_handle_input_redirection(t_ms *ms)
{
	pid_t	pid;

	if (ms->in_fd != -1)
	{
		dup2(ms->in_fd, STDIN_FILENO);
		close(ms->in_fd);
	}
	else if (ms->delim)
	{
		pid = fork();
		if (pid == -1)
			ft_error();
		else if (pid == 0)
		{
			ft_heredoc(ms->delim, 0);
			exit(0);
		}
		else
			waitpid(pid, &ms->exit_status, 0);
	}
}

