/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabriela <gabriela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 09:13:58 by gabriela          #+#    #+#             */
/*   Updated: 2023/10/04 13:31:46 by gabriela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_child_process(int prev_fd, t_cmd *current, t_ms *ms)
{
	int	in_fd;
	int	out_fd;

	in_fd = -1;
	out_fd = -1;
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (current->input_redir)
	{
		in_fd = open(current->input_redir, O_RDONLY | O_CLOEXEC, 0777);
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (current->next)
	{
		dup2(ms->pipe_fd[1], STDOUT_FILENO);
		close(ms->pipe_fd[1]);
	}
	else if (current->output_redir)
	{
		out_fd = open(current->output_redir, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0777);
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	else if (current->append_output_redir)
	{
		out_fd = open(current->append_output_redir, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0777);
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	else if (ft_check_builtin(current->cmd))
		ft_execute_forked_builtin(current, ms);
	if (execve(current->cmd, current->arg, environ) == -1)
		ft_command_not_found(current);
	exit(0);
}

/* Close read end of previous pipe. If there is next cmd,
store read end of current pipe in prev_fd. If there is no
next command, close write end of current pipe.
Wait for child process to finish. */
void	ft_parent_process(int *prev_fd, int pid, t_ms *ms, t_cmd *current)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (current->next)
		*prev_fd = ms->pipe_fd[0];
	close(ms->pipe_fd[1]);
	waitpid(pid, &ms->exit_status, 0);
}

/* Check if there is a pipe with previous command. */
int	ft_check_if_piped(t_cmd *current, int prev_fd)
{
	int	is_piped;

	if ((current->next != NULL) || (prev_fd != -1))
		is_piped = 1;
	else
		is_piped = 0;
	return (is_piped);
}

void	ft_execute_cmd(t_ms *ms, t_cmd *current)
{
	int		prev_fd;
	int		pid;
	int		is_piped;
	int		heredoc_fd[2];

	prev_fd = -1;
	while (current)
	{
		if (current->here_doc)
		{
			ft_heredoc(current->here_doc, heredoc_fd);
			prev_fd = heredoc_fd[0];
		}
		is_piped = ft_check_if_piped(current, prev_fd);
		if (!is_piped && ft_check_builtin(current->cmd))
		{
			ft_execute_builtin_directly(current, ms);
			current = current->next;
			continue ;
		}
		if (current->next && pipe(ms->pipe_fd) == -1)
			ft_error();
		pid = fork();
		if (pid == -1)
			ft_error();
		if (pid == 0)
			ft_child_process(prev_fd, current, ms);
		else
			ft_parent_process(&prev_fd, pid, ms, current);
		current = current->next;
	}
}
