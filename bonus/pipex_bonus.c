/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:17:00 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/02/19 19:50:45 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	handle_child_process(t_pipex *pipex, int i, int in_fd, int *pipe_fd)
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

static void	handle_parent_process(pid_t pid, int *in_fd, int *pipe_fd, int i)
{
	int	status;

	waitpid(pid, &status, 0);
	if (*in_fd != -1)
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

static void	process_pipeline_command(t_pipex *pipex, int i, int *in_fd)
{
	int		pipe_fd[2];
	pid_t	pid;

	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (i < pipex->cmd_count - 1)
	{
		if (pipe(pipe_fd) == -1)
			ft_handle_errors("pipex", "pipe creation failed", NULL, 1);
	}
	pid = fork();
	if (pid == -1)
		ft_handle_errors("pipex", "fork failed", NULL, 1);
	if (pid == 0)
		handle_child_process(pipex, i, *in_fd, pipe_fd);
	else
		handle_parent_process(pid, in_fd, pipe_fd, i);
}

static int	execute_pipeline(t_pipex *pipex)
{
	int	i;
	int	in_fd;

	in_fd = pipex->infile_fd;
	i = 0;
	while (i < pipex->cmd_count)
		process_pipeline_command(pipex, i++, &in_fd);
	if (in_fd != -1)
		close(in_fd);
	if (pipex->outfile_fd != -1)
		close(pipex->outfile_fd);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*pipex;

	if (argc < 2)
	{
		ft_printf("Usage: " USAGE);
		ft_handle_errors("pipex", "invalid number of arguments", NULL, 1);
	}
	pipex = parse_input(argc, argv, envp);
	execute_pipeline(pipex);
	free_pipex(pipex);
	return (0);
}
