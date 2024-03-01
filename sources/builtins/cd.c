/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygurma <ygurma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 16:23:17 by ygurma            #+#    #+#             */
/*   Updated: 2024/02/26 13:20:25 by ygurma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
 * Check if the 'cd' command has an argument (directory path)
*/
int builtin_cd(char **args) {
    const char *cd_error = "minishell: cd: missing operand\n";
    const char *chdir_error = "minishell: cd error\n";
    int exit_status = 0; // Success

    if (args[1] == NULL) {
        write(STDERR_FILENO, cd_error, strlen(cd_error));
        exit_status = 1; // Failure due to missing operand
    } else {
        if (chdir(args[1]) != 0) {
            write(STDERR_FILENO, chdir_error, strlen(chdir_error));
            exit_status = 1; // Failure due to chdir() error
        }
    }

    return exit_status;
}

