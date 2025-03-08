/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:27:28 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/08 12:54:21 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	execute_command(t_command *cmd, t_context *ctx)
{
	char	*path;

	path = find_executable(cmd->argv[0], ctx->envp);
	if (!path)
		error_exit_code(127, "command not found", cmd->argv[0], ctx);
	execve(path, cmd->argv, ctx->envp);
	error_exit_code(126, strerror(errno), path, ctx);
	return (-1);
}
