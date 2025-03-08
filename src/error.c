/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:27:22 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/09 00:01:49 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_exit_code(int code, char *msg, char *target, t_context *ctx)
{
	ctx->exit_code = code;
	ft_putstr_fd(ctx->prog_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(target, 2);
	if (target)
		ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putchar_fd('\n', 2);
	close_all_fds(ctx);
	exit(ctx->exit_code);
}
