#include "minishell.h"
/*
 *
*/

void process_quoted_string(char **cursor, char quote_char, char **token_start) {
    char *write_ptr = *cursor; // Use this to write back characters if needed
    (*cursor)++; // Skip the opening quote

    *token_start = write_ptr; // Adjusted: The start of the quoted content for the output

    while (**cursor) {
        if (**cursor == '\\' && *(*cursor + 1) == quote_char) {
            // Skip over the escape character
            (*cursor)++;
            // Copy the escaped character (the quote)
            *write_ptr = *(*cursor);
            write_ptr++;
            (*cursor)++;
        } else if (**cursor == quote_char) {
            // Found the closing quote
            break;
        } else {
            // Copy the current character
            *write_ptr = **cursor;
            write_ptr++;
            (*cursor)++;
        }
    }

    // Null-terminate the token
    *write_ptr = '\0';

    if (**cursor == quote_char) {
        (*cursor)++; // Skip the closing quote
    }
}



void add_processed_token(char **tokens, int *position, int *bufsize, char *token_start, char *cursor) {
    int token_length = cursor - token_start;
    char *raw_token = strndup(token_start, token_length);
    if (!raw_token) handle_allocation_error();

    // Process the token (function to remove escape characters to be implemented)
    char *token = process_token(raw_token);
    free(raw_token);

    tokens[*position] = token;
    (*position)++;
    if (*position >= *bufsize) {
        resize_token_array(&tokens, bufsize, *position);
    }
}

/**
 * Removes escape characters from a token.
 * 
 * @param raw_token The token to process.
 * @return A new string with escape characters removed.
 */
char *process_token(const char *raw_token) {
    if (raw_token == NULL) {
        return NULL;
    }

    size_t len = strlen(raw_token);
    char *processed_token = malloc(len + 1);
    if (processed_token == NULL) {
        return NULL;
    }

    const char *src = raw_token;
    char *dst = processed_token;

    while (*src) {
        if (*src == '\\') {
            src++; // Skip the escape character
            if (*src) {
                *dst++ = *src++; // Copy the next character
            }
        } else {
            *dst++ = *src++; // Copy current character
        }
    }

    *dst = '\0'; // Null-terminate the processed token
    return processed_token;
}
