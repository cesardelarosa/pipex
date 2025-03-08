/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:27:40 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/08 19:31:26 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*generate_temp_name(void)
{
	char	*name;

	name = ft_strdup("/tmp/pipex_heredoc");
	return (name);
}

static int	create_temp_file(char **temp_file, t_context *ctx)
{
	*temp_file = generate_temp_name();
	if (!*temp_file)
	{
		error_exit_code(1, "malloc failed", NULL, ctx);
		return (0);
	}
	return (1);
}

static int	read_input_to_file(int fd, char *delimiter)
{
	char	*line;
	char	*cmp_line;

	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		cmp_line = ft_strtrim(line, "\n");
		if (ft_strcmp(cmp_line, delimiter) == 0)
		{
			free(cmp_line);
			free(line);
			break ;
		}
		free(cmp_line);
		ft_putstr_fd(line, fd);
		free(line);
	}
	return (1);
}

static void	cleanup_temp_file(int fd, char *temp_file, t_context *ctx)
{
	close(fd);
	fd = open(temp_file, O_RDONLY);
	if (fd < 0)
	{
		free(temp_file);
		error_exit_code(1, strerror(errno), "heredoc", ctx);
	}
	unlink(temp_file);
	free(temp_file);
	register_fd(fd, ctx);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		error_exit_code(1, strerror(errno), "heredoc dup2", ctx);
	}
}

int	handle_heredoc(t_redir *redir, t_context *ctx)
{
	char	*temp_file;
	int		fd;

	if (!create_temp_file(&temp_file, ctx))
		return (-1);
	fd = open(temp_file, O_RDWR | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(temp_file);
		error_exit_code(1, strerror(errno), "heredoc", ctx);
	}
	if (!read_input_to_file(fd, redir->file))
	{
		close(fd);
		free(temp_file);
		error_exit_code(1, "warning: heredoc delimited by end-of-file",
			redir->file, ctx);
	}
	cleanup_temp_file(fd, temp_file, ctx);
	return (0);
}
