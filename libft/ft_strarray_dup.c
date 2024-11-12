/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strarray_dup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:55:31 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/10/28 11:55:32 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	**ft_strarray_dup(char **tokens, int start, int end)
{
	int		i;
	int		len;
	char	**dup;

	len = end - start + 1;
	dup = malloc((len + 1) * sizeof(char *));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = ft_strdup(tokens[start + i]);
		if (dup[i++] == NULL)
			ft_free_split(dup);
	}
	dup[len] = NULL;
	return (dup);
}
