/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygurma <ygurma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 17:48:31 by ygurma            #+#    #+#             */
/*   Updated: 2024/03/01 15:08:04 by ygurma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Enters the main loop of the shell, handling user input and executing commands.
 */
void	shell_loop(void)
{
	char	*input_line;
	char	**args;
	int		last_exit_status;

	last_exit_status = 0;
	setup_signal_handlers();
	while (1)
	{
		if (g_sigint_received)
		{
			g_sigint_received = 0;
			continue ;
		}
		input_line = readline("minishell> ");
		if (input_line == NULL)
		{
			write(STDOUT_FILENO, "\n", 1);
			exit(EXIT_SUCCESS);
		}
		if (*input_line)
		{
			args = read_and_parse_input(input_line);
			last_exit_status = execute_and_free_commands(args,
					last_exit_status);
		}
		free(input_line);
	}
}

/**
 * Parses the input string from the user into arguments for execution.
 * @param input_line The input string from the user.
 * @return An array of parsed arguments.
 */
char	**read_and_parse_input(char *input_line)
{
	char	**args;

	args = NULL;
	if (*input_line)
	{
		add_history(input_line);
		args = parse_input(input_line);
	}
	return (args);
}

/**
 * Executes the given command arguments and frees the allocated memory.
 * @param args The command arguments to execute.
 * @param last_exit_status The exit status of the last executed command.
 * @return The exit status of the executed command.
 */
int	execute_and_free_commands(char **args, int last_exit_status)
{
	int	i;
	int	command_exit_status;

	i = 0;
	command_exit_status = execute_command(args, last_exit_status);
	while (args[i] != NULL)
	{
		free(args[i]);
		i++;
	}
	free(args);
	return (command_exit_status);
}

/**
 * The main function of the shell program.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @param envp Environment pointer.
 * @return Exit status.
 */
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	if (argc != 1)
	{
		write(STDERR_FILENO, "Usage: minishell\n", 18);
		return (1);
	}
	setup_signal_handlers();
	shell_loop();
	return (0);
}
