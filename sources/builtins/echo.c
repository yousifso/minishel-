/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygurma <ygurma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 17:00:42 by ygurma            #+#    #+#             */
/*   Updated: 2024/02/26 14:12:51 by ygurma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Prints the given arguments to standard output.
 * Supports the '-n' option to not output the trailing newline.
 * 
 * @param args The command-line arguments array where args[0] is "echo", 
 *             followed by optional '-n' and the strings to be printed.
 * @return Returns 0 to indicate successful execution.
 */
int builtin_echo(char **args)
{
    int newline = 1; // By default, echo prints a newline after the output

    // Check for the '-n' option which instructs echo not to print a newline at the end
    if (args[1] && strcmp(args[1], "-n") == 0)
    {
        newline = 0; // Do not print a newline after the output
        args++; // Skip the '-n' option for the rest of processing
    }
    args++; // Skip the command name "echo" to start with the actual message

    // Iterate through the remaining arguments to print them
    while (*args != NULL)
    {
        printf("%s", *args); // Print the current argument
        if (*(args + 1) != NULL)
        {
            printf(" "); // Print a space between arguments
        }
        args++; // Move to the next argument
    }

    // Print a newline unless '-n' option was provided
    if (newline)
    {
        printf("\n");
    }

    return 0; // Indicate successful execution
}

