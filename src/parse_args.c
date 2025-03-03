/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:12:58 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/04 00:13:30 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static void	process_quoted(const char **str, char *buffer, int *i)
{
	char	quote;

	quote = **str;
	(*str)++;
	while (**str && **str != quote)
	{
		if (quote == '\"' && **str == '\\')
		{
			(*str)++;
			if (!**str)
				break ;
		}
		buffer[(*i)++] = **str;
		(*str)++;
	}
	if (**str == quote)
		(*str)++;
}

static void	process_backslash(const char **str, char *buffer, int *i)
{
	(*str)++;
	if (**str)
	{
		buffer[(*i)++] = **str;
		(*str)++;
	}
}

static char	*extract_token(const char **str)
{
	char	*buffer;
	char	*token;
	int		i;

	while (**str == ' ' || **str == '\t')
		(*str)++;
	if (!**str)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * (ft_strlen(*str) + 1));
	if (!buffer)
		return (NULL);
	i = 0;
	while (**str && **str != ' ' && **str != '\t')
	{
		if (**str == '\'' || **str == '\"')
			process_quoted(str, buffer, &i);
		else if (**str == '\\')
			process_backslash(str, buffer, &i);
		else
			buffer[i++] = *(*str)++;
	}
	buffer[i] = '\0';
	token = ft_strdup(buffer);
	free(buffer);
	return (token);
}

static int	count_tokens(const char *input)
{
	int			count;
	char		*token;
	const char	*temp;

	temp = input;
	count = 0;
	token = extract_token(&temp);
	while (token != NULL)
	{
		free(token);
		count++;
		token = extract_token(&temp);
	}
	return (count);
}

char	**parse_args(const char *input)
{
	char		**args;
	char		*token;
	const char	*temp;
	int			i;

	if (!input)
		return (NULL);
	args = ft_calloc(count_tokens(input) + 1, sizeof(char *));
	if (!args)
		return (NULL);
	temp = input;
	i = 0;
	token = extract_token(&temp);
	while (token != NULL)
	{
		args[i++] = token;
		token = extract_token(&temp);
	}
	args[i] = NULL;
	return (args);
}
