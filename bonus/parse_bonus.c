/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:56:21 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/02/19 19:46:40 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	open_file(t_pipex *pipex, int argc, char **argv)
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

static char	**parse_commands(char **argv, int start, int cmd_count)
{
	char	**cmds;
	int		i;

	cmds = malloc(sizeof(char *) * (cmd_count + 1));
	if (!cmds)
		ft_handle_errors("pipex", "malloc failed", NULL, 1);
	i = 0;
	while (i < cmd_count)
	{
		cmds[i] = argv[start + i];
		i++;
	}
	cmds[i] = NULL;
	return (cmds);
}

static int	check_infile(char *infile)
{
	int	fd;

	if (access(infile, F_OK) == -1)
		ft_handle_errors("pipex", "no such file", infile, 1);
	if (access(infile, R_OK) == -1)
		ft_handle_errors("pipex", "permission denied", infile, 1);
	fd = open(infile, O_RDONLY);
	if (fd < 0)
		ft_handle_errors("pipex", "error opening file", infile, 1);
	return (fd);
}

t_pipex	*parse_input(int argc, char **argv, char **envp)
{
	t_pipex	*pipex;
	int		heredoc;

	heredoc = !ft_strcmp(argv[1], "here_doc");
	if (argc < 5 + heredoc)
	{
		ft_printf("Usage: " USAGE);
		ft_handle_errors("pipex", "invalid number of arguments", NULL, 1);
	}
	pipex = init_pipex();
	if (heredoc)
	{
		pipex->limiter = argv[2];
		setup_here_doc(pipex);
	}
	else
	{
		pipex->limiter = NULL;
		pipex->infile_fd = check_infile(argv[1]);
	}
	pipex->envp = envp;
	pipex->cmd_count = argc - 3 - heredoc;
	pipex->cmds = parse_commands(argv, 2 + heredoc, pipex->cmd_count);
	open_file(pipex, argc, argv);
	return (pipex);
}
