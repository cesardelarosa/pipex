/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_child_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:11:50 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/11/12 12:11:51 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	execute_command(char *cmd, char **envp)
{
	char	*args[4];
	char	*sh;

	sh = "/bin/sh";
	args[0] = sh;
	args[1] = "-c";
	args[2] = cmd;
	args[3] = NULL;
	if (execve(sh, args, envp) == -1)
		ft_handle_errors("pipex", "command not found", cmd, 127);
}

void	child_process(t_pipex *px, char *cmd, int *prev_fd, int *pipe_fd)
{
	if (prev_fd != NULL)
	{
		dup_with_error_check(prev_fd[0], STDIN_FILENO);
		close(prev_fd[0]);
	}
	else if (px->here_doc)
		handle_here_doc(px);
	else
		setup_infile(px);
	if (pipe_fd != NULL)
	{
		dup_with_error_check(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	else
		setup_outfile(px);
	close_pipes(prev_fd, pipe_fd);
	execute_command(cmd, px->envp);
}

void	setup_infile(t_pipex *px)
{
	int	infile_fd;

	infile_fd = open(px->infile, O_RDONLY);
	if (infile_fd < 0)
		ft_handle_errors("pipex", "no such file or directory", px->infile, 1);
	dup_with_error_check(infile_fd, STDIN_FILENO);
	close(infile_fd);
}

void	setup_outfile(t_pipex *px)
{
	int	outfile_fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (px->here_doc)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	outfile_fd = open(px->outfile, flags, 0644);
	if (outfile_fd < 0)
		ft_handle_errors("pipex", "permission denied", px->outfile, 1);
	dup_with_error_check(outfile_fd, STDOUT_FILENO);
	close(outfile_fd);
}

void	handle_here_doc(t_pipex *px)
{
	char	*line;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		ft_handle_errors("pipex", "error creating pipe for heredoc", NULL, 1);
	while (1)
	{
		ft_putstr_fd("heredoc> ", 1);
		line = get_next_line(STDIN_FILENO);
		if (!line || ft_strncmp(line, px->argv[2], ft_strlen(px->argv[2])) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipe_fd[1]);
		free(line);
	}
	close(pipe_fd[1]);
	dup_with_error_check(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
}
