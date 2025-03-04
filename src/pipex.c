/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@42madrid.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 12:18:54 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/05/23 12:18:54 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include "libft.h"
#include "pipex.h"

static void	dup_with_error_check(int oldfd, int newfd, char *context)
{
	if (dup2(oldfd, newfd) >= 0)
		return ;
	close(oldfd);
	ft_handle_errors("pipex", "dup2 failed", context, 1);
}

static void	child_process(int *pipe_fd, char **argv, char **envp)
{
	int	infile;

	infile = open(argv[1], O_RDONLY);
	if (infile < 0)
		ft_handle_errors("pipex", strerror(errno), argv[1], 1);
	close(pipe_fd[0]);
	dup_with_error_check(infile, STDIN_FILENO, "infile");
	dup_with_error_check(pipe_fd[1], STDOUT_FILENO, "pipe output");
	close(pipe_fd[1]);
	close(infile);
	execute_command(argv[2], envp);
}

static void	parent_process(int *pipe_fd, char **argv, char **envp)
{
	int	outfile;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
		ft_handle_errors("pipex", strerror(errno), argv[4], 1);
	close(pipe_fd[1]);
	dup_with_error_check(pipe_fd[0], STDIN_FILENO, "pipe input");
	dup_with_error_check(outfile, STDOUT_FILENO, "outfile");
	close(pipe_fd[0]);
	close(outfile);
	execute_command(argv[3], envp);
}

static int	wait_children(pid_t pid[2])
{
	int	status[2];
	int	exit_code;

	waitpid(pid[0], &status[0], 0);
	waitpid(pid[1], &status[1], 0);
	if (WIFEXITED(status[1]))
		exit_code = WEXITSTATUS(status[1]);
	else
		exit_code = 1;
	return (exit_code);
}

int	main(int argc, char **argv, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid[2];

	if (argc != 5)
	{
		ft_printf("Usage: " USAGE "\n");
		ft_handle_errors("pipex", "invalid number of arguments", NULL, 1);
	}
	if (pipe(pipe_fd) == -1)
		ft_handle_errors("pipex", "pipe creation failed", NULL, 1);
	pid[0] = fork();
	if (pid[0] == -1)
		ft_handle_errors("pipex", "fork failed", NULL, 1);
	if (pid[0] == 0)
		child_process(pipe_fd, argv, envp);
	pid[1] = fork();
	if (pid[1] == -1)
		ft_handle_errors("pipex", "fork failed", NULL, 1);
	if (pid[1] == 0)
		parent_process(pipe_fd, argv, envp);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (wait_children(pid));
}
