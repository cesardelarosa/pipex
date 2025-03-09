/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:28:14 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/09 15:21:17 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct_creation.h"

t_pipeline	*pipeline_create(void)
{
	t_pipeline	*pipeline;

	pipeline = ft_calloc(1, sizeof(t_pipeline));
	return (pipeline);
}

void	pipeline_destroy(t_pipeline *pipeline)
{
	int	i;

	if (pipeline->pipes)
	{
		i = 0;
		while (i < pipeline->cmd_count - 1)
		{
			free(pipeline->pipes[i]);
			i++;
		}
		free(pipeline->pipes);
	}
	free(pipeline->pids);
	ft_lstclear(&pipeline->commands, command_destroy);
	free(pipeline);
}

int	pipeline_add_command(t_pipeline *p, t_command *cmd)
{
	t_list	*node;

	node = ft_lstnew(cmd);
	if (!node)
		return (0);
	ft_lstadd_back(&p->commands, node);
	p->cmd_count++;
	cmd->p = p;
	return (1);
}
