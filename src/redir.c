/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:28:31 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/08 13:01:21 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "handle_redir.h"

t_redir	*redir_create(t_redir_type type, char *file)
{
	t_redir	*r;

	r = ft_calloc(1, sizeof(t_redir));
	if (!r)
		return (NULL);
	r->type = type;
	r->file = ft_strdup(file);
	if (!r->file)
	{
		free(r);
		return (NULL);
	}
	return (r);
}

void	redir_destroy(void *content)
{
	t_redir	*r;

	r = (t_redir *)content;
	free(r->file);
	free(r);
}

int	handle_redirs(t_list *redirs, t_context *ctx)
{
	t_redir	*r;

	while (redirs)
	{
		r = (t_redir *)redirs->content;
		if (r->type == REDIR_INPUT && handle_redir_in(r, ctx) < 0)
			return (-1);
		else if (r->type == REDIR_OUTPUT && handle_redir_out(r, ctx) < 0)
			return (-1);
		else if (r->type == REDIR_APPEND && handle_redir_append(r, ctx) < 0)
			return (-1);
		else if (r->type == REDIR_HEREDOC && handle_heredoc(r, ctx) < 0)
			return (-1);
		redirs = redirs->next;
	}
	return (0);
}
