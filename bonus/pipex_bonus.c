/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:17:00 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/02/20 10:51:13 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

t_pipex	*init_pipex(void)
{
	t_pipex	*pipex;

	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		ft_handle_errors("pipex", "malloc failed", NULL, 1);
	ft_bzero(pipex, sizeof(t_pipex));
	return (pipex);
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

	pipex = init_pipex();
	parse_input(argc, argv, envp, pipex);
	exit = execute_pipeline(pipex);
	free_pipex(pipex);
	return (exit);
}
