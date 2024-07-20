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

#include "pipex.h"

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
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

void	child_process(int *pipe_fd, char **argv, char **envp)
{
	int	infile;

	infile = open(argv[1], O_RDONLY);
	if (infile < 0)
		handle_errors("Error opening input file", argv[1]);
	close(pipe_fd[0]);
	if (dup2(infile, STDIN_FILENO) < 0)
		handle_errors("dup2 infile error", NULL);
	if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
		handle_errors("dup2 pipe error", NULL);
	close(pipe_fd[1]);
	close(infile);
	execute_command(argv[2], envp);
}

void	parent_process(int *pipe_fd, char **argv, char **envp)
{
	int	outfile;
	int	status;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
		handle_errors("Error opening output file", argv[4]);
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
		handle_errors("dup2 pipe error", NULL);
	if (dup2(outfile, STDOUT_FILENO) < 0)
		handle_errors("dup2 outfile error", NULL);
	close(pipe_fd[0]);
	close(outfile);
	execute_command(argv[3], envp);
	waitpid(-1, &status, 0);
}

int	main(int argc, char **argv, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
		return (1);
	}
	if (pipe(pipe_fd) == -1)
		handle_errors("Pipe error", NULL);
	pid = fork();
	if (pid == -1)
		handle_errors("Fork error", NULL);
	if (pid == 0)
		child_process(pipe_fd, argv, envp);
	else
		parent_process(pipe_fd, argv, envp);
	return (0);
}
