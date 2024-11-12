/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@42madrid.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 12:26:34 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/05/23 12:26:34 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "libft.h"

void	child_process(int *pipe_fd, char **argv, char **envp);
void	parent_process(int *pipe_fd, char **argv, char **envp);
char	*get_command_path(char *cmd, char **envp);
void	handle_errors(char *msg, char *cmd);
char	*join_paths(const char *path, const char *cmd);
char	**get_paths_from_envp(char **envp);

#endif
