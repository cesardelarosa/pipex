/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 13:20:58 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/11/17 13:20:58 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct_bonus.h"

static void	read_here_doc(const char *delimiter, int write_fd)
{
	char	*line;

	while (1)
	{
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		line[ft_strcspn(line, "\n")] = '\0';
		if (ft_strcmp(line, (char *)delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
}

void	setup_here_doc(t_pipex *pipex)
{
	int	here_doc_pipe[2];

	if (pipe(here_doc_pipe) == -1)
		ft_handle_errors("pipex", "pipe creation failed", NULL, 1);
	read_here_doc(pipex->limiter, here_doc_pipe[1]);
	pipex->infile_fd = here_doc_pipe[0];
	close(here_doc_pipe[1]);
}
