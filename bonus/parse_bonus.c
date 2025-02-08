/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:56:21 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/11/26 17:56:21 by cde-la-r         ###   ########.fr       */
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
		pipex->limiter = NULL;
		if (access(argv[1], F_OK) == -1)
			ft_handle_errors("pipex", "no such file", argv[1], 1);
		if (access(argv[1], R_OK) == -1)
			ft_handle_errors("pipex", "permission denied", argv[1], 1);
		pipex->infile_fd = open(argv[1], O_RDONLY);
		if (pipex->infile_fd < 0)
			ft_handle_errors("pipex", "error opening file", argv[1], 1);
	}
}

char	**parse_commands(char **argv, int start, int cmd_count)
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
