/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:28:24 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/09 15:40:46 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

void	close_all_pipes(t_pipeline *p)
{
	unsigned int	i;

	if (!p->pipes)
		return ;
	i = 0;
	while (i + 1 < p->cmd_count)
	{
		close(p->pipes[i][0]);
		close(p->pipes[i][1]);
		i++;
	}
}

void	safe_close(int *fd)
{
	if (*fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}
