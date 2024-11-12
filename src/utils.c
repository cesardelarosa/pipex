/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@42madrid.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 12:28:44 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/05/23 12:28:44 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "pipex.h"

void	handle_errors(char *msg, char *cmd)
{
	if (cmd)
		ft_printf("pipex: %s: %s\n", cmd, msg);
	else
		perror(msg);
	exit(EXIT_FAILURE);
}

char	*join_paths(const char *path, const char *cmd)
{
	char	*temp_path;
	char	*full_path;

	temp_path = ft_strjoin(path, "/");
	if (!temp_path)
		return (NULL);
	full_path = ft_strjoin(temp_path, cmd);
	free(temp_path);
	return (full_path);
}

char	**get_paths_from_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	return (ft_split(envp[i] + 5, ':'));
}

char	*get_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;

	paths = get_paths_from_envp(envp);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path = join_paths(paths[i], cmd);
		if (!path)
			break ;
		if (access(path, X_OK) == 0)
		{
			ft_free_split(paths);
			return (path);
		}
		free(path);
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}
