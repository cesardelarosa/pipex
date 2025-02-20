/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:51:33 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/02/20 11:51:58 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
	if (pipex->pid_arr)
	{
		free(pipex->pid_arr);
		pipex->pid_arr = NULL;
	}
	free(pipex);
}

void	free_and_exit(t_pipex *pipex, char *msg, char *cmd, int exit_code)
{
	if (pipex)
		free_pipex(pipex);
	ft_handle_errors("pipex", msg, cmd, exit_code);
}
