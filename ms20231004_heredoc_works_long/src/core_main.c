/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabriela <gabriela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 08:20:57 by gabriela          #+#    #+#             */
/*   Updated: 2023/10/04 13:25:55 by gabriela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Handle the case where Ctrl+D is pressed to exit.
If input is not empty, add it to the history. 
Reset the fields in the ms structure. Break input 
string into tokens and identifz commands, arguments 
and other stuff. If there is at least 1 command, try
to execute it. */
void	ft_handle_input(char *input, t_ms *ms, char *prompt)
{
	if (!input)
	{
		printf("exit\n");
		free(prompt);
		exit(0);
	}
	else if (input[0] != '\0')
	{
		free(prompt);
		add_history(input);
		ft_reset(ms);
		ft_lex_and_parse_input_string(input, ms);
		ms->i_last_cmd = ft_get_command_count(ms->cmd_head) - 1;
		if (ms->i_last_cmd >= 0)
			ft_executor(ms);
	}
}

/* Prepare the input by marking spaces outside of quotes and braces.
Replace special symbols with their corresponding value.
Store tokens in the ms structure.
Process redirections.
Tokenize the command and its arguments.
Process strings within quotes. 
Process unquoted environment variables.
Check the command type and free the tokens array.
*/
void	ft_lex_and_parse_input_string(const char *in_str, t_ms *ms)
{
	char	**tokens;

	tokens = ft_split(ft_transform_for_lexer(in_str), TOKEN_DELIMITER);
	ft_replace_symbols(ms);
	ms->tokens = tokens;
	tokenize_cmd_and_args(tokens, ms);
	ft_handle_quoted_env_vars(ms);
	ft_handle_unquoted_env_vars(ms);
	ft_check_cmd_type(ms);
	free_array(tokens);
}

/* Run the commands that have been parsed and stored
in the linked list. Duplicate the fd for standard input
and output, so that they can be changed back later. 
Handle input redirection. Execute each command in the
pipeline and redirect input and output between the
commands as needed. Handle output redirection. Restore
the original output and input to previously saved fds.
*/
void	ft_executor(t_ms *ms)
{
	t_cmd	*current;

	ft_replace_exit_status(ms->cmd_head, 0, ms->exit_status);
	ms->saved_stdin = dup(STDIN_FILENO);
	ms->saved_stdout = dup(STDOUT_FILENO);
	current = ms->cmd_head;
	ft_execute_cmd(ms, current);
	ft_restore_input_output(ms);
	free_cmd_list(ms->cmd_head);
}


int	main(void)
{
	t_ms	ms;
	char	*input;
	char	*prompt;

	ft_init(&ms);
//	signal(SIGINT, ft_sigint_handler);
//	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		prompt = ft_strjoin(getenv("USER"), " ^⨀ ᴥ ⨀ ^ ");
		input = readline(prompt);
		ft_handle_input(input, &ms, prompt);
		free(input);
	}
	return (0);
}
