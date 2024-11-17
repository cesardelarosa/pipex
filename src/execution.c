/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 13:50:47 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/11/17 13:50:48 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

void	execute_command(char *cmd, char **envp)
{
	char	*args[4];
	char	*sh;

	sh = "/bin/sh";
	args[0] = sh;
	args[1] = "-c";
	args[2] = cmd;
	args[3] = NULL;
	if (execve(sh, args, envp) == -1)
		ft_handle_errors("pipex", "command not found", cmd, 127);
}
