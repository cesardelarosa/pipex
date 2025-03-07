/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:55:03 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/07 18:51:02 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_error(const char *s1, const char *s2, const char *s3)
{
	if (s1)
	{
		ft_putstr_fd((char *)s1, 2);
		if (s2 || s3)
			ft_putstr_fd(": ", 2);
	}
	if (s2)
	{
		ft_putstr_fd((char *)s2, 2);
		if (s3)
			ft_putstr_fd(": ", 2);
	}
	if (s3)
		ft_putstr_fd((char *)s3, 2);
	ft_putstr_fd("\n", 2);
}
