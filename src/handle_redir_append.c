/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir_append.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:27:45 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/08 17:58:43 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	handle_redir_append(t_redir *redir, t_context *ctx)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		error_exit_code(1, strerror(errno), redir->file, ctx);
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		error_exit_code(1, strerror(errno), "dup2", ctx);
	}
	close(fd);
	return (0);
}
