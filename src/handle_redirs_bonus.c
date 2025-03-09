/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:27:28 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/09 14:17:14 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "handle_redir_bonus.h"

int	handle_redirs(t_list *redirs)
{
	t_redir	*r;

	while (redirs)
	{
		r = (t_redir *)redirs->content;
		if (r->type == REDIR_INPUT && handle_redir_in(r) < 0)
			return (-1);
		else if (r->type == REDIR_OUTPUT && handle_redir_out(r) < 0)
			return (-1);
		else if (r->type == REDIR_APPEND && handle_redir_append(r) < 0)
			return (-1);
		else if (r->type == REDIR_HEREDOC && handle_heredoc(r) < 0)
			return (-1);
		redirs = redirs->next;
	}
	return (0);
}
