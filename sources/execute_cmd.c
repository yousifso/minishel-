/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygurma <ygurma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 13:38:10 by ygurma            #+#    #+#             */
/*   Updated: 2024/03/01 12:07:35 by ygurma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Returns a table mapping builtin 
 * command names to their corresponding functions.
 */
static t_command	*get_builtin_cmds(void)
{
	static t_command	builtin_cmds[] = {
	{"cd", &builtin_cd},
	{"echo", &builtin_echo},
	{"export", &builtin_export},
	{"exit", &builtin_exit},
	{"pwd", &builtin_pwd},
	{NULL, NULL}
	};

	return (builtin_cmds);
}

/**
 * Executes a builtin command based on the command name found in args[0].
 * Updates exit_status with the result of the command execution.
 * Returns 1 if a builtin command was executed, 0 otherwise.
 */
static int	execute_builtin(char **args, int *exit_status)
{
	t_command	*builtin_cmds;
	int			i;

	i = 0;
	builtin_cmds = get_builtin_cmds();
	while (builtin_cmds[i].name != NULL)
	{
		if (strcmp(args[0], builtin_cmds[i].name) == 0)
		{
			*exit_status = builtin_cmds[i].func(args);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	wait_for_child(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		return (WEXITSTATUS(status));
	}
	else
	{
		return (128 + WTERMSIG(status));
	}
}

static int	execute_child_process(char **args)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	apply_redirections(args);
	if (execvp(args[0], args) == -1)
	{
		perror("minishell: command not found");
		exit(127);
	}
	return (0);
}

/**
 * Executes an external command using execvp.
 * Handles SIGINT and SIGQUIT signals by setting them to default.
 * Applies redirections for the command if necessary.
 * Returns the exit status of the executed command.
 */
static int	execute_external_cmd(char **args)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		return (execute_child_process(args));
	}
	else if (pid < 0)
	{
		perror("minishell: fork");
		exit(EXIT_FAILURE);
	}
	else
	{
		return (wait_for_child(pid));
	}
}

/**
 * Main function to execute a command.
 * Checks if the command is "echo $?", 
 * a builtin, or an external command and executes it.
 * Restores the original stdout and 
 * stdin file descriptors after execution.
 * Returns the exit status of the executed command.
 */	
int	execute_command(char **args, int last_exit_status)
{
	int	exit_status;
	int	original_stdout;
	int	original_stdin;

	exit_status = 0;
	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);
	apply_redirections(args);
	if (strcmp(args[0], "echo") == 0
		&& args[1] != NULL && strcmp(args[1], "$?") == 0)
	{
		printf("%d\n", last_exit_status);
	}
	else
	{
		if (execute_builtin(args, &exit_status) == 0)
		{
			exit_status = execute_external_cmd(args);
		}
	}
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdout);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	return (exit_status);
}
