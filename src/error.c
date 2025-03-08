/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:27:22 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/08 12:53:31 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	put_error_part(const char *str)
{
	if (str)
		ft_putstr_fd((char *)str, 2);
}

void	error_exit(char *msg, char *target, t_context *ctx)
{
	put_error_part(ctx->prog_name);
	ft_putstr_fd(": ", 2);
	put_error_part(target);
	if (target)
		ft_putstr_fd(": ", 2);
	put_error_part(msg);
	ft_putchar_fd('\n', 2);
	close_all_fds(ctx);
	exit(ctx->exit_code);
}

void	error_exit_code(int code, char *msg, char *target, t_context *ctx)
{
	ctx->exit_code = code;
	error_exit(msg, target, ctx);
}
