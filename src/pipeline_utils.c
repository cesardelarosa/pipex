/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:28:24 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/08 13:01:00 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_all_pipes(t_pipeline *p)
{
	int	i;

	if (!p->pipes)
		return ;
	i = 0;
	while (i < p->cmd_count - 1)
	{
		close(p->pipes[i][0]);
		close(p->pipes[i][1]);
		i++;
	}
}

void	cleanup_pipeline(t_pipeline *p)
{
	int	i;

	if (p->pipes)
	{
		i = 0;
		while (i < p->cmd_count - 1)
		{
			free(p->pipes[i]);
			i++;
		}
		free(p->pipes);
	}
	if (p->pids)
		free(p->pids);
}

void	safe_close(int *fd)
{
	if (*fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}
