/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygurma <ygurma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 21:46:12 by ygurma            #+#    #+#             */
/*   Updated: 2024/02/26 14:19:33 by ygurma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
 * Implements the export functionality of the shell.
 * Iterates over environment variables and prints them with "declare -x" prefix.
 * 
 * @param env The environment variables array.
 * @return Always returns 0 to indicate success.
 */
int builtin_export(char **env) {
    // Loop through each environment variable.
    for (int i = 0; env[i] != NULL; i++) {
        // Print the "declare -x" prefix to indicate export.
        const char *prefix = "declare -x ";
        write(STDOUT_FILENO, prefix, strlen(prefix));
        // Print the environment variable name and value.
        write(STDOUT_FILENO, env[i], strlen(env[i]));
        // Print a newline character after each environment variable.
        write(STDOUT_FILENO, "\n", 1);
    }
    // Return 0 to indicate the command executed successfully.
    return 0;
}