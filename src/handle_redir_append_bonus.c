/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir_append_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 12:15:28 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/10 12:15:30 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "structs.h"
#include "errors.h"

int	handle_redir_append(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		error_exit_code(1, strerror(errno), redir->file, redir->cmd->p);
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
