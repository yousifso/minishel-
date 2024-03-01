/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yousifgurma <yousifgurma@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:53:20 by ygurma            #+#    #+#             */
/*   Updated: 2024/02/21 17:38:40 by yousifgurma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Global flag to indicate SIGINT reception. 
 * 'volatile' ensures the variable is updated
 * across different contexts, and 'sig_atomic_t' 
 * guarantees atomic access to prevent data races.
 */
volatile sig_atomic_t	g_sigint_received = 0;

/*
 * Handler for SIGINT signal (ctrl-C)
 * Prevent unused parameter warning
*/
void	sigint_handler(int sig)
{
	(void)sig;
	g_sigint_received = 1;
	rl_replace_line("", 0);
	write(STDERR_FILENO, "\nminishell> ", 12);
	rl_on_new_line();
	rl_redisplay();
}

/*
 * Use (void)sig to avoid unused variable warning.
*/
void	sigquit_handler(int sig)
{
	(void)sig;
}

/*
 * nitialize to zero for SIGQUIT
 * Initialize to zero for SIGQUIT
 * Setup SIGINT handling
 * Ignore SIGQUIT
*/
void	setup_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	memset(&sa_int, 0, sizeof(sa_int));
	memset(&sa_quit, 0, sizeof(sa_quit));
	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
