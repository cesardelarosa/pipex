/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:17:00 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/02/19 22:18:33 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	child_process(t_pipex *pipex, int i, int in_fd, int pipe_fd[2])
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
}

static void	launch_command(t_pipex *pipex, int i, int *in_fd, pid_t *pid_arr)
{
	int		pipe_fd[2];

	ft_bzero(pipe_fd, sizeof(pipe_fd));
	if (i < pipex->cmd_count - 1 && pipe(pipe_fd) == -1)
		ft_handle_errors("pipex", "pipe creation failed", NULL, 1);
	pid_arr[i] = fork();
	if (pid_arr[i] == -1)
		ft_handle_errors("pipex", "fork failed", NULL, 1);
	if (pid_arr[i] == 0)
	{
		child_process(pipex, i, *in_fd, pipe_fd);
		execute_command(pipex->cmds[i], pipex->envp);
	}
	close(*in_fd);
	if (i < pipex->cmd_count - 1)
	{
		close(pipe_fd[1]);
		*in_fd = pipe_fd[0];
	}
}

static int	wait_pids(pid_t *pid_arr, int n)
{
	int	i;
	int	status;

	i = 0;
	while (i < n)
	{
		waitpid(pid_arr[i], &status, 0);
		if (i++ == n - 1)
			return (status);
	}
	return (0);
}

static int	execute_pipeline(t_pipex *pipex)
{
	int		i;
	int		in_fd;
	pid_t	*pid_arr;
	int		status;

	pid_arr = malloc(sizeof(pid_t) * pipex->cmd_count);
	if (!pid_arr)
		ft_handle_errors("pipex", "malloc failed", NULL, 1);
	in_fd = pipex->infile_fd;
	i = 0;
	while (i < pipex->cmd_count)
		launch_command(pipex, i++, &in_fd, pid_arr);
	if (in_fd != -1)
		close(in_fd);
	if (pipex->outfile_fd != -1)
		close(pipex->outfile_fd);
	status = wait_pids(pid_arr, pipex->cmd_count);
	free(pid_arr);
	return (WEXITSTATUS(status));
}

void	free_pipex(t_pipex *pipex)
{
	if (pipex->cmds)
	{
		free(pipex->cmds);
		pipex->cmds = NULL;
	}
	if (pipex->infile_fd >= 0)
	{
		close(pipex->infile_fd);
		pipex->infile_fd = -1;
	}
	if (pipex->outfile_fd >= 0)
	{
		close(pipex->outfile_fd);
		pipex->outfile_fd = -1;
	}
	free(pipex);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*pipex;
	int		exit;

	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		ft_handle_errors("pipex", "malloc failed", NULL, 1);
	ft_bzero(pipex, sizeof(t_pipex));
	parse_input(argc, argv, envp, pipex);
	exit = execute_pipeline(pipex);
	free_pipex(pipex);
	return (exit);
}
