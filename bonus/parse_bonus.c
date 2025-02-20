/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:56:21 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/02/20 11:57:34 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	open_file(t_pipex *pipex, int argc, char **argv, int heredoc)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (heredoc)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	pipex->outfile_fd = open(argv[argc - 1], flags, 0644);
	if (pipex->outfile_fd < 0)
		free_and_exit(pipex, "error opening file", argv[argc - 1], 1);
}

static char	**parse_commands(char **argv, int start, t_pipex *pipex)
{
	char	**cmds;
	int		i;

	cmds = malloc(sizeof(char *) * (pipex->cmd_count + 1));
	if (!cmds)
		free_and_exit(pipex, "malloc failed", NULL, 1);
	i = 0;
	while (i < pipex->cmd_count)
	{
		cmds[i] = argv[start + i];
		i++;
	}
	cmds[i] = NULL;
	return (cmds);
}

static void	setup_here_doc(t_pipex *pipex)
{
	int		p[2];
	char	*line;

	if (pipe(p) == -1)
		free_and_exit(pipex, "pipe creation failed", NULL, 1);
	while (1)
	{
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		line[ft_strcspn(line, "\n")] = '\0';
		if (!ft_strcmp(line, pipex->limiter))
		{
			free(line);
			break ;
		}
		write(p[1], line, ft_strlen(line));
		write(p[1], "\n", 1);
		free(line);
	}
	close(p[1]);
	pipex->infile_fd = p[0];
}

static int	check_infile(char *infile, t_pipex *pipex)
{
	int	fd;

	if (access(infile, F_OK) == -1)
		free_and_exit(pipex, "no such file", infile, 1);
	if (access(infile, R_OK) == -1)
		free_and_exit(pipex, "permission denied", infile, 1);
	fd = open(infile, O_RDONLY);
	if (fd < 0)
		free_and_exit(pipex, "error opening file", infile, 1);
	return (fd);
}

void	parse_input(int argc, char **argv, char **envp, t_pipex *pipex)
{
	int		heredoc;

	heredoc = argc > 1 && ft_strcmp(argv[1], "here_doc") == 0;
	if (argc < 5 + heredoc)
	{
		ft_printf("Usage: " USAGE "\n");
		free_and_exit(pipex, "invalid number of arguments", NULL, 1);
	}
	if (heredoc)
	{
		pipex->limiter = argv[2];
		setup_here_doc(pipex);
	}
	else
	{
		pipex->limiter = NULL;
		pipex->infile_fd = check_infile(argv[1], pipex);
	}
	pipex->envp = envp;
	pipex->cmd_count = argc - 3 - heredoc;
	pipex->cmds = parse_commands(argv, 2 + heredoc, pipex);
	open_file(pipex, argc, argv, heredoc);
}
