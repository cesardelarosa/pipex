/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:10:28 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/10/28 12:10:28 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtok(char *str, const char *delim)
{
	static char	*saved;
	char		*token;

	if (str == NULL)
		str = saved;
	if (str == NULL)
		return (NULL);
	while (*str && ft_strchr(delim, *str))
		str++;
	if (*str == NULL)
	{
		saved = NULL;
		return (NULL);
	}
	token = str;
	while (*str != NULL && ft_strchr(delim, *str) == 0)
		str++;
	if (*str != NULL)
	{
		*str = '\0';
		saved = str + 1;
	}
	else
		saved = NULL;
	return (token);
}
