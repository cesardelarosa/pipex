/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:17:00 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/11/15 00:17:00 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "pipex_bonus.h"

void	open_files(t_pipex *pipex, int argc, char **argv)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (pipex->here_doc)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	pipex->outfile_fd = open(argv[argc - 1], flags, 0644);
	if (pipex->outfile_fd < 0)
		ft_handle_errors("pipex", "error opening file", argv[argc - 1], 1);
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

int	execute_pipeline(t_pipex *pipex)
{
	int	i;
	int	in_fd;

	in_fd = pipex->infile_fd;
	i = 0;
	while (i < pipex->cmd_count)
	{
		process_pipeline_command(pipex, i, &in_fd);
		i++;
	}
	if (in_fd != -1)
		close(in_fd);
	if (pipex->outfile_fd != -1)
		close(pipex->outfile_fd);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc < 5)
	{
		ft_printf("Usage: ./pipex [\"here_doc\" <delimiter>]/[<infile>] <cmd1>"
			" ... <cmdN> <outfile>\n");
		ft_handle_errors("pipex", "invalid number of arguments", NULL, 1);
	}
	pipex.envp = envp;
	parse_input(&pipex, argc, argv);
	open_files(&pipex, argc, argv);
	pipex.cmd_count = argc - 3 - pipex.here_doc;
	pipex.cmds = parse_commands(argv, 2 + pipex.here_doc, pipex.cmd_count);
	execute_pipeline(&pipex);
	free_pipex(&pipex);
	return (0);
}
