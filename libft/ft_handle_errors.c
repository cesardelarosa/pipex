/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 09:43:12 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/11/12 09:43:12 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

void	ft_handle_errors(char *program, char *msg, char *cmd, int status)
{
	if (cmd && program)
		ft_printf("%s: %s: %s\n", program, cmd, msg);
	else
		perror(msg);
	exit(status);
}
