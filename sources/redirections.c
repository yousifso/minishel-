/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygurma <ygurma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 13:41:12 by ygurma            #+#    #+#             */
/*   Updated: 2024/02/24 15:13:30 by ygurma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Redirects standard input to a file
void redirect_input(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("minishell: open input file");
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

// Redirects standard output to a file
void redirect_output(const char *filename, int append) {
    int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
    int fd = open(filename, flags, 0644);
    if (fd < 0) {
        perror("minishell: open output file");
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

// Handles execution of a single command with redirections if needed
void execute_command_with_redirections(char **command) {
    apply_redirections(command);
    if (execvp(command[0], command) == -1) {
        perror("minishell: execvp");
        exit(EXIT_FAILURE);
    }
}

// Forks and executes a command with optional input and output redirection.
void fork_and_execute(char **command, int input_fd, int output_fd) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        
        // Redirect input if specified
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        // Redirect output if specified
        if (output_fd != STDOUT_FILENO) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        // Execute the command
        execute_command_with_redirections(command);
    } else if (pid < 0) {
        // Forking failed
        perror("minishell: fork");
        exit(EXIT_FAILURE);
    }

    // Parent process waits for the child to complete
    waitpid(pid, NULL, 0);

    // Close the input and output file descriptors in the parent, if they were redirected
    if (input_fd != STDIN_FILENO) {
        close(input_fd);
    }
    if (output_fd != STDOUT_FILENO) {
        close(output_fd);
    }
}


void apply_redirections(char **args) {
    for (int i = 0; args[i]; ++i) {
        if (!strcmp(args[i], ">")) {
            redirect_output(args[i + 1], 0);
            args[i] = NULL; // Terminate command args before redirection symbol.
            break;
        } else if (!strcmp(args[i], ">>")) {
            redirect_output(args[i + 1], 1);
            args[i] = NULL;
            break;
        } else if (!strcmp(args[i], "<")) {
            redirect_input(args[i + 1]);
            args[i] = NULL;
            break;
        } else if (!strcmp(args[i], "<<")) {
            redirect_input(args[i + 1]);
            args[i] = NULL;
            break;
        }
    }
}

// Main function to manage pipes between commands
void pipe_commands(char **commands, int num_commands) {
    int fd_in = 0, fds[2];

    for (int i = 0; i < num_commands - 1; ++i) {
        pipe(fds);
        fork_and_execute(&commands[i], fd_in, fds[1]);
        close(fds[1]);
        if (fd_in != 0) close(fd_in);
        fd_in = fds[0];
    }

    // Last command
    if (fd_in != 0) {
        dup2(fd_in, STDIN_FILENO);
    }
    fork_and_execute(&commands[num_commands - 1], fd_in, STDOUT_FILENO);
    if (fd_in != 0) close(fd_in);
}
