/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:27:40 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/08 23:27:11 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "structs.h"
#include "errors.h"

static void	write_heredoc_lines(int fd, t_redir *redir)
{
	char	*line;
	int		tty_fd;

	while (1)
	{
		tty_fd = open("/dev/tty", O_WRONLY);
		if (tty_fd < 0)
			break ;
		ft_putstr_fd("> ", tty_fd);
		close(tty_fd);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

int	handle_heredoc(t_redir *redir)
{
	int			fd;
	const char	*tmp_path = "/tmp/.heredoc_tmp";

	fd = open(tmp_path, O_RDWR | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		error_exit_code(1, strerror(errno), "open");
	write_heredoc_lines(fd, redir);
	close(fd);
	fd = open(tmp_path, O_RDONLY);
	if (fd < 0)
		error_exit_code(1, strerror(errno), "open");
	unlink(tmp_path);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
