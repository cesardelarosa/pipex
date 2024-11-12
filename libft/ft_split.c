/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 21:21:30 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/08/29 18:30:13 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_split_size(const char *s, char c)
{
	int	size;

	if (s == NULL)
		return (0);
	size = 0;
	while (*s++)
		size += (size == 0 || *(s - 2) == c) && *(s - 1) != c;
	return (size);
}

char	**ft_free_split(char **result)
{
	int	i;

	if (!result)
		return (NULL);
	i = 0;
	while (result[i])
		free(result[i++]);
	free(result);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int			size;
	char		**result;
	int			i;
	const char	*start;

	if (s == NULL)
		return (NULL);
	size = ft_split_size(s, c);
	result = (char **)malloc((size + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (*s && i < size)
	{
		while (*s == c)
			s++;
		start = s;
		while (*s && *s != c)
			s++;
		result[i++] = ft_substr(start, 0, s - start);
		if (!result[i - 1])
			return (ft_free_split(result));
	}
	result[i] = NULL;
	return (result);
}
