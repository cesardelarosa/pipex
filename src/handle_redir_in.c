/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir_in.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:27:50 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/08 12:57:04 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	handle_redir_in(t_redir *redir, t_context *ctx)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd < 0)
		error_exit(strerror(errno), redir->file, ctx);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		error_exit(strerror(errno), "dup2", ctx);
	}
	register_fd(fd, ctx);
	return (0);
}
