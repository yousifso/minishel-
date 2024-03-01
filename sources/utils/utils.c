/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygurma <ygurma@19.be>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 12:07:41 by ygurma            #+#    #+#             */
/*   Updated: 2023/12/28 16:46:04 by ygurma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *skip_closing_quote(char *cursor, char quote_char) {
    return quote_char && *cursor == quote_char ? cursor + 1 : cursor;
}

void handle_allocation_error() {
    fprintf(stderr, "minishell: allocation error\n");
    exit(EXIT_FAILURE);
}

/**
 * Skips leading whitespace in the input string.
 * Returns a pointer to the first non-whitespace character.
 */
char *skip_whitespace(char *cursor) {
    while (*cursor && isspace((unsigned char)*cursor)) {
        cursor++;
    }
    return cursor;
}

int safe_atoi(const char *str, int *is_valid) {
    long val;
    char *endptr;
    errno = 0;
    val = strtol(str, &endptr, 10);
    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
        || (errno != 0 && val == 0) || endptr == str || *endptr != '\0'
        || val < INT_MIN || val > INT_MAX) {
        *is_valid = 0;
        return 0;
    } else {
        *is_valid = 1;
        return (int)val;
    }
}
