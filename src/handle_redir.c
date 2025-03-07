/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:55:03 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/07 18:34:02 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <fcntl.h>
#include <errno.h>
#include <string.h>

static int	handle_redir_input(t_redir *redir)
{
	int	fd;

	if (!redir || !redir->file)
		return (-1);
	fd = open(redir->file, O_RDONLY);
	if (fd < 0)
	{
		ft_print_error("pipex", redir->file, strerror(errno));
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		ft_print_error("pipex:", strerror(errno), NULL);
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_redir_output(t_redir *redir)
{
	int	fd;

	if (!redir || !redir->file)
		return (-1);
	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_print_error("pipex", redir->file, strerror(errno));
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		ft_print_error("pipex", strerror(errno), NULL);
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_redir(t_redir *redir)
{
	if (!redir)
		return (0);
	if (redir->type == REDIR_INPUT)
		return (handle_redir_input(redir));
	else if (redir->type == REDIR_OUTPUT)
		return (handle_redir_output(redir));
	return (-1);
}
