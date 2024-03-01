/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygurma <ygurma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 15:50:00 by ygurma            #+#    #+#             */
/*   Updated: 2024/02/27 20:19:21 by ygurma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Standard Library Library 
# include <stdio.h>
# include <signal.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h> 
# include <ctype.h> // For isspace
# include <limits.h>
# include <sys/wait.h>
# include <errno.h>

// Struct for mapping command names to their handeling functions 
typedef struct s_command {
	const char	*name;
	int	(*func)(char **args);
}	t_command;

/*
 * To make it accessible to other sources file
*/
extern volatile sig_atomic_t	g_sigint_received;

// Function declarations
void	parse_command(char *input);
int	execute_command(char **args, int last_exit_status);
int	execute_and_free_commands(char **args, int last_exit_status);
void	handle_builtin(char **args);
char	**parse_input(char *input);
char	*find_special_token(char *cursor, char **token_start);
char	*find_token_end(char *cursor, char quote_char);
void	resize_token_array(char ***tokens, int *bufsize, int position);
char	*skip_whitespace(char *cursor);
char	*expand_env_variable(char *cursor, char **token_start);
void	redirect_input(const char *filename);
void	redirect_output(const char *filename, int append);
void	pipe_commands(char **commands, int num_commands);
int	tokenize_input(char *temp, char **tokens, int *bufsize, int position);
char	*expand_env_variable(char *cursor, char **token_start);
char	*skip_whitespace(char *cursor);
char	**read_and_parse_input(char *input_line);
void	shell_loop(void);
void apply_redirections(char **args);
void fork_and_execute(char **command, int input_fd, int output_fd);


// builtins functions
int	builtin_echo(char **args);
int	builtin_cd(char **args);
int	builtin_pwd(char **args);
int	builtin_export(char **env);
int	builtin_exit(char **args);

/* tokenization functions
char	**tokenize_input(char *input);
char	*find_token_end(char *str);
*/

// signal_handler
void	setup_signal_handlers(void);
void	sigquit_handler(int sig);
void	sigint_handler(int sig);

// utils && utils1
char	*skip_closing_quote(char *cursor, char quote_char);
void	handle_allocation_error(void);
int	safe_atoi(const char *str, int *is_valid);
void	add_processed_token(char **tokens, int *position, int *bufsize, char *token_start, char *cursor);
void	process_quoted_string(char **cursor, char quote_char, char **token_start);
char	*process_token(const char *raw_token);

#endif
