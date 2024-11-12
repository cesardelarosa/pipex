/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_error_exit.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 23:21:26 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/11/11 23:21:26 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

void	ft_error_exit_free(char *msg, int status, void *to_free, \
			void (*free_func)(void *))
{
	if (to_free && free_func)
		free_func(to_free);
	if (msg)
		perror(msg);
	exit(status);
}
