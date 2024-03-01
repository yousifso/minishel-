/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygurma <ygurma@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:12:58 by ygurma            #+#    #+#             */
/*   Updated: 2024/02/20 13:19:39 by ygurma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Exits the shell program. 
 * If a numeric argument is provided, exits with that status code. 
 * If the argument is not valid, prints an error message and returns 0 without exiting.
 * 
 * @param args The command-line arguments array where args[0] is "exit" and args[1], if present, is the exit status.
 * @return Returns 0 if the exit status argument is invalid, indicating the shell should continue running.
 */
int	builtin_exit(char **args)
{
	int	status;
	int	is_valid;
	// Check if an argument is provided for the exit status.
	if (args[1])
	{
		// Convert the argument to an integer and check validity.
		status = safe_atoi(args[1], &is_valid);
		// If the argument is not a valid number, print an error and return 0.
		if (!is_valid)
		{
			write(STDERR_FILENO, "exit: numeric argument required\n", 32);
			return 0;
		}
		// Exit with the provided status if it's valid.
		exit(status);
	}
	else
	{
		// If no argument is provided, exit with success status.
		exit(EXIT_SUCCESS);
	}
}
