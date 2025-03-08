/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:26:26 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/08 13:06:43 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_command	*command_create(char *cmd_str)
{
	t_command	*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = parse_args(cmd_str);
	if (!cmd->argv)
	{
		free(cmd);
		return (NULL);
	}
	cmd->redirs = NULL;
	return (cmd);
}

void	command_destroy(void *content)
{
	t_command	*cmd;

	cmd = (t_command *)content;
	ft_free_split(cmd->argv);
	ft_lstclear(&cmd->redirs, redir_destroy);
	free(cmd);
}

int	command_add_redir(t_command *cmd, t_redir_type type, char *file)
{
	t_redir	*r;
	t_list	*node;

	r = redir_create(type, file);
	if (!r)
		return (0);
	node = ft_lstnew(r);
	if (!node)
	{
		redir_destroy(r);
		return (0);
	}
	ft_lstadd_back(&cmd->redirs, node);
	return (1);
}
