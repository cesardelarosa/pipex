/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 10:48:37 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/02/20 10:49:05 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	child_process(t_pipex *pipex, int i, int in_fd, int pipe_fd[2])
{
	if (dup2(in_fd, STDIN_FILENO) == -1)
		free_and_exit(pipex, "dup2 failed", "input", 1);
	close(in_fd);
	if (i < pipex->cmd_count - 1)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			free_and_exit(pipex, "dup2 failed", "output", 1);
		close(pipe_fd[1]);
		close(pipe_fd[0]);
	}
	else
	{
		if (dup2(pipex->outfile_fd, STDOUT_FILENO) == -1)
			free_and_exit(pipex, "dup2 failed", "outfile", 1);
		close(pipex->outfile_fd);
	}
}

static void	launch_command(t_pipex *pipex, int i, int *in_fd)
{
	int		pipe_fd[2];

	ft_bzero(pipe_fd, sizeof(pipe_fd));
	if (i < pipex->cmd_count - 1 && pipe(pipe_fd) == -1)
		free_and_exit(pipex, "pipe creation failed", NULL, 1);
	pipex->pid_arr[i] = fork();
	if (pipex->pid_arr[i] == -1)
		free_and_exit(pipex, "fork failed", NULL, 1);
	if (pipex->pid_arr[i] == 0)
	{
		child_process(pipex, i, *in_fd, pipe_fd);
		execute_command(pipex, i);
	}
	close(*in_fd);
	if (i < pipex->cmd_count - 1)
	{
		close(pipe_fd[1]);
		*in_fd = pipe_fd[0];
	}
}

static int	wait_pids(t_pipex *pipex, int n)
{
	int	i;
	int	status;

	i = 0;
	while (i < n)
	{
		waitpid(pipex->pid_arr[i], &status, 0);
		if (i++ == n - 1)
			return (status);
	}
	return (0);
}

int	execute_pipeline(t_pipex *pipex)
{
	int		i;
	int		in_fd;
	int		status;

	pipex->pid_arr = malloc(sizeof(pid_t) * pipex->cmd_count);
	if (!pipex->pid_arr)
		free_and_exit(pipex, "malloc failed", NULL, 1);
	in_fd = pipex->infile_fd;
	i = 0;
	while (i < pipex->cmd_count)
		launch_command(pipex, i++, &in_fd);
	if (in_fd != -1)
		close(in_fd);
	if (pipex->outfile_fd != -1)
		close(pipex->outfile_fd);
	status = wait_pids(pipex, pipex->cmd_count);
	return (WEXITSTATUS(status));
}
