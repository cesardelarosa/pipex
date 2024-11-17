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

void	parse_input(t_pipex *pipex, int argc, char **argv)
{
	if (argc >= 6 && ft_strcmp(argv[1], "here_doc") == 0)
	{
		pipex->here_doc = 1;
		pipex->limiter = argv[2];
		setup_here_doc(pipex);
	}
	else
	{
		pipex->here_doc = 0;
		if (access(argv[1], F_OK) == -1)
			ft_handle_errors("pipex", "no such file", argv[1], 1);
		if (access(argv[1], R_OK) == -1)
			ft_handle_errors("pipex", "permission denied", argv[1], 1);
		pipex->infile_fd = open(argv[1], O_RDONLY);
		if (pipex->infile_fd < 0)
			ft_handle_errors("pipex", "error opening file", argv[1], 1);
	}
}

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

char	**parse_commands(char **argv, int start, int cmd_count)
{
	char	**cmds;
	int		i;

	cmds = malloc(sizeof(char *) * (cmd_count + 1));
	if (!cmds)
		ft_handle_errors("pipex", "malloc failed", NULL, 1);
	i = -1;
	while (i++ < cmd_count)
		cmds[i] = argv[start + i];
	cmds[i] = NULL;
	return (cmds);
}

int	execute_pipeline(t_pipex *pipex)
{
	int		i;
	int		in_fd;
	int		pipe_fd[2];
	pid_t	pid;

	in_fd = pipex->infile_fd;
	i = 0;
	while (i < pipex->cmd_count)
	{
		if (i < pipex->cmd_count - 1 && pipe(pipe_fd) == -1)
			ft_handle_errors("pipex", "pipe creation failed", NULL, 1);
		pid = fork();
		if (pid == -1)
			ft_handle_errors("pipex", "fork failed", NULL, 1);
		if (pid == 0)
			handle_child_process(pipex, i, in_fd, pipe_fd);
		else
			handle_parent_process(pid, &in_fd, pipe_fd, i++);
	}
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
	pipex.here_doc = 0;
	pipex.envp = envp;
	parse_input(&pipex, argc, argv);
	open_files(&pipex, argc, argv);
	pipex.cmd_count = argc - 3 - pipex.here_doc;
	pipex.cmds = parse_commands(argv, 2 + pipex.here_doc, pipex.cmd_count);
	execute_pipeline(&pipex);
	close(pipex.infile_fd);
	close(pipex.outfile_fd);
	return (0);
}
