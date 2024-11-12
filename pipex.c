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
#include "libft.h"

static void	execute_command(char *cmd, char **envp)
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

	if (access(argv[1], F_OK) == -1)
		ft_handle_errors("pipex", "no such file or directory", argv[1], 1);
	if (access(argv[1], R_OK) == -1)
		ft_handle_errors("pipex", "permission denied", argv[1], 1);
	infile = open(argv[1], O_RDONLY);
	if (infile < 0)
		ft_handle_errors("pipex", "error opening file", argv[1], 1);
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

	if (access(argv[4], W_OK) == -1 && access(argv[4], F_OK) == 0)
		ft_handle_errors("pipex", "permission denied", argv[4], 1);
	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
		ft_handle_errors("pipex", "error opening file", argv[4], 1);
	close(pipe_fd[1]);
	dup_with_error_check(pipe_fd[0], STDIN_FILENO, "pipe input");
	dup_with_error_check(outfile, STDOUT_FILENO, "outfile");
	close(pipe_fd[0]);
	close(outfile);
	execute_command(argv[3], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (argc != 5)
	{
		ft_printf("Use: ./pipex <infile> <cmd1> <cmd2> <outfile>\n");
		ft_handle_errors("pipex", "invalid number of arguments", NULL, 1);
	}
	if (pipe(pipe_fd) == -1)
		ft_handle_errors("pipex", "pipe creation failed", NULL, 1);
	pid = fork();
	if (pid == -1)
		ft_handle_errors("pipex", "fork failed", NULL, 1);
	if (pid == 0)
		child_process(pipe_fd, argv, envp);
	else
	{
		parent_process(pipe_fd, argv, envp);
		waitpid(-1, &status, 0);
	}
	return (status);
}
