/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir_out.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:27:55 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/08 13:58:53 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	handle_redir_out(t_redir *redir, t_context *ctx)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		error_exit_code(errno_exit_code(), strerror(errno), redir->file, ctx);
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		error_exit_code(errno_exit_code(), strerror(errno), "dup2", ctx);
	}
	register_fd(fd, ctx);
	return (0);
}
