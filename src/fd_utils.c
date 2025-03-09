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

	if (fd < 0)
		return ;
	fd_ptr = ft_calloc(1, sizeof(int));
	if (!fd_ptr)
	{
		close(fd);
		return ;
	}
	*fd_ptr = fd;
	node = ft_lstnew(fd_ptr);
	if (!node)
	{
		free(fd_ptr);
		close(fd);
		return ;
	}
	ft_lstadd_back(&ctx->open_fds, node);
}

void	close_all_fds(t_context *ctx)
{
	t_list	*node;
	t_list	*next;
	int		fd;

	node = ctx->open_fds;
	while (node)
	{
		fd = *(int *)node->content;
		if (fd >= 0)
			close(fd);
		free(node->content);
		next = node->next;
		free(node);
		node = next;
	}
	ctx->open_fds = NULL;
}

void	safe_close(int *fd)
{
	if (*fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}
