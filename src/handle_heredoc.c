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

#include "pipex.h"

static void	write_heredoc_lines(int fd, t_redir *redir)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("> ", 1);
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

int	handle_heredoc(t_redir *redir, t_context *ctx)
{
	int			fd;
	int			dup_ret;
	const char	*tmp_path = "/tmp/.heredoc_tmp";

	fd = open(tmp_path, O_RDWR | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		error_exit_code(1, strerror(errno), "open", ctx);
	write_heredoc_lines(fd, redir);
	close(fd);
	fd = open(tmp_path, O_RDONLY);
	if (fd < 0)
		error_exit_code(1, strerror(errno), "open", ctx);
	unlink(tmp_path);
	dup_ret = dup2(fd, STDIN_FILENO);
	if (dup_ret < 0)
	{
		close(fd);
		error_exit_code(1, strerror(errno), "dup2", ctx);
	}
	register_fd(fd, ctx);
	return (0);
}
