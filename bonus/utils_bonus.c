/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:17:00 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/11/15 00:17:00 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	handle_child_process(t_pipex *pipex, int i, int in_fd, int *pipe_fd)
{
	if (dup2(in_fd, STDIN_FILENO) == -1)
		ft_handle_errors("pipex", "dup2 failed", "input", 1);
	close(in_fd);
	if (i < pipex->cmd_count - 1)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			ft_handle_errors("pipex", "dup2 failed", "output", 1);
		close(pipe_fd[1]);
		close(pipe_fd[0]);
	}
	else
	{
		if (dup2(pipex->outfile_fd, STDOUT_FILENO) == -1)
			ft_handle_errors("pipex", "dup2 failed", "outfile", 1);
		close(pipex->outfile_fd);
	}
	execute_command(pipex->cmds[i], pipex->envp);
}

void	handle_parent_process(pid_t pid, int *in_fd, int *pipe_fd, int i)
{
	int	status;

	waitpid(pid, &status, 0);
	if (i >= 0 && *in_fd != -1)
	{
		close(*in_fd);
		*in_fd = -1;
	}
	if (pipe_fd[0] != -1)
	{
		*in_fd = pipe_fd[0];
		close(pipe_fd[1]);
	}
}
