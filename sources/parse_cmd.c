/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygurma <ygurma@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 17:10:00 by ygurma            #+#    #+#             */
/*   Updated: 2024/03/01 14:13:24 by ygurma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Parse the input string into tokens
char	**parse_input(char *input)
{
	int		bufsize;
	int		position;
	char	**tokens;
	char	*temp;

	bufsize = 64;
	position = 0;
	tokens = malloc(bufsize * sizeof(char *));
	if (!tokens)
	{
		handle_allocation_error();
	}
	temp = strdup(input);
	if (!temp)
	{
		free(tokens);
		handle_allocation_error();
	}
	position = tokenize_input(temp, tokens, &bufsize, position);
	free(temp);
	return (tokens);
}

// Tokenize the input string based on whitespace, quotes, and special characters

int	tokenize_input(char *temp, char **tokens, int *bufsize, int position)
{
	char	*cursor;
	char	*token_start;

	cursor = temp;
	while (*cursor)
	{
		cursor = skip_whitespace(cursor);
		if (*cursor == '\0')
			break ;
		token_start = cursor;
		if ((*cursor == '>' && *(cursor + 1) == '>')
			|| (*cursor == '<' && *(cursor + 1) == '<'))
		{
			cursor += 2;
		}
		else if (*cursor == '>' || *cursor == '<' || *cursor == '|')
		{
			cursor += 1;
		}
		else if (*cursor == '\"' || *cursor == '\'')
		{
			cursor++;
			token_start = cursor;
			while (*cursor && *cursor != '\"' && *cursor != '\'')
				cursor++;
		}
		else
		{
			while (*cursor && !isspace((unsigned char)*cursor)
				&& *cursor != '<' && *cursor != '>' && *cursor != '|'
				&& *cursor != '\"' && *cursor != '\'')
			{
				cursor++;
			}
		}
		add_processed_token(tokens, &position, bufsize, token_start, cursor);
		if (*cursor == '\"' || *cursor == '\'')
			cursor++;
	}
	tokens[position] = NULL;
	return (position);
}

/*
*** Identify and handle special characters (redirections, pipes) in the token ***
* Single-character token (>, <, |)
* Return the token before the special character
* Implement this function
*/
char	*find_special_token(char *cursor, char **token_start)
{
	while (*cursor)
	{
		if (*cursor == '<' || *cursor == '>' || *cursor == '|')
		{
			if (*token_start == cursor)
			{
				return (cursor + 1);
			}
			else
			{
				return (cursor);
			}
		}
		else if (*cursor == '$')
		{
			if (isalnum(*(cursor + 1)) || *(cursor + 1) == '-')
			{
				cursor = expand_env_variable(cursor, token_start);
				continue ;
			}
		}
		else if (isspace((unsigned char)*cursor))
		{
			return (cursor);
		}
		else if (*cursor == '\\' && (*(cursor + 1) == '\"'
				|| *(cursor + 1) == '\'' || *(cursor + 1) == '$'))
		{
			cursor++;
		}
		cursor++;
	}
	return (cursor);
}

char	*expand_env_variable(char *cursor, char **token_start)
{
	char	*end;
	char	*var_name;
	char	*var_value;

	end = cursor;
	var_value = getenv(var_name);
	var_name = strndup(cursor + 1, end - cursor - 1);
	while (*end && (isalnum((unsigned char)*end) || *end == '_'))
	{
		end++;
	}
	if (!var_name)
	{
		handle_allocation_error();
	}
	free(var_name);
	if (var_value)
	{
		*token_start = strdup(var_value);
		if (!*token_start)
		{
			handle_allocation_error();
		}
	}
	return (end);
}

char	*find_token_end(char *cursor, char quote_char)
{
	if (quote_char)
	{
		while (*cursor && *cursor != quote_char)
		{
			cursor++;
		}
		return (cursor);
	}
	while (*cursor && !isspace((unsigned char)*cursor)
		&& *cursor != '<' && *cursor != '>' && *cursor != '|')
	{
		cursor++;
	}
	return (cursor);
}

// Resize the token array dynamically if more space is needed
void	resize_token_array(char ***tokens, int *bufsize, int position)
{
	char	**new_tokens;

	new_tokens = realloc(*tokens, (*bufsize) * sizeof(char *));
	if (position >= *bufsize)
	{
		*bufsize += 64;
		if (!new_tokens)
		{
			handle_allocation_error();
		}
		*tokens = new_tokens;
	}
}
