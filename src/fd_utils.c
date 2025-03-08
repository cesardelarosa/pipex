/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:27:33 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/08 12:55:21 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	register_fd(int fd, t_context *ctx)
{
	int		*fd_ptr;
	t_list	*node;

	fd_ptr = ft_calloc(1, sizeof(int));
	*fd_ptr = fd;
	node = ft_lstnew(fd_ptr);
	ft_lstadd_back(&ctx->open_fds, node);
}

void	close_all_fds(t_context *ctx)
{
	t_list	*node;

	node = ctx->open_fds;
	while (node)
	{
		close(*(int *)node->content);
		node = node->next;
	}
}
