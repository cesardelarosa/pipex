/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 13:50:47 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/04 18:33:03 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "libft.h"
#include "pipex.h"

static char	*join_path(const char *dir, const char *cmd)
{
	char	*path;
	size_t	len_dir;
	size_t	len_cmd;

	len_dir = ft_strlen(dir);
	len_cmd = ft_strlen(cmd);
	path = (char *)malloc((len_dir + len_cmd + 2) * sizeof(char));
	if (!path)
		return (NULL);
	ft_memcpy(path, dir, len_dir);
	path[len_dir] = '/';
	ft_memcpy(path + len_dir + 1, cmd, len_cmd);
	path[len_dir + len_cmd + 1] = '\0';
	return (path);
}

static char	*get_path_env(char **envp)
{
	while (*envp && ft_strncmp(*envp, "PATH=", 5))
		envp++;
	if (!*envp)
		return (".");
	return (*envp + 5);
}

static char	*search_in_path(char *cmd, char **paths)
{
	char	*full_path;
	int		found_not_executable;

	found_not_executable = 0;
	while (*paths)
	{
		full_path = join_path(*paths, cmd);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		if (access(full_path, F_OK) == 0)
			found_not_executable = 1;
		free(full_path);
		paths++;
	}
	if (found_not_executable)
		ft_handle_errors("pipex", "permission denied", cmd, 126);
	return (NULL);
}

static char	*find_exec(char *cmd, char **envp)
{
	char	**paths;
	char	*path_env;
	char	*exec_path;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		if (access(cmd, F_OK) == 0)
			ft_handle_errors("pipex", "permission denied", cmd, 126);
		return (NULL);
	}
	path_env = get_path_env(envp);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	exec_path = search_in_path(cmd, paths);
	ft_free_split(paths);
	return (exec_path);
}

void	execute_command(char *cmd, char **envp)
{
	char	**args;
	char	*exec_path;
	int		exit_code;

	args = parse_args(cmd);
	if (!args)
		ft_handle_errors("pipex", "malloc error", NULL, 1);
	exec_path = find_exec(args[0], envp);
	if (!exec_path)
	{
		ft_free_split(args);
		ft_handle_errors("pipex", "command not found", cmd, 127);
	}
	execve(exec_path, args, envp);
	if (errno == EACCES || errno == EISDIR)
		exit_code = 126;
	else if (errno == ENOENT)
		exit_code = 127;
	else
		exit_code = 1;
	ft_handle_errors("pipex", strerror(errno), args[0], exit_code);
}
