/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:17:00 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/11/15 00:17:00 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "struct_bonus.h"

void	free_pipex(t_pipex *pipex);
void	setup_here_doc(t_pipex *pipex);
void	handle_child_process(t_pipex *pipex, int i, int in_fd, int *pipe_fd);
void	handle_parent_process(pid_t pid, int *in_fd, int *pipe_fd, int i);
void	execute_command(char *cmd, char **envp);
void	parse_input(t_pipex *pipex, int argc, char **argv);
char	**parse_commands(char **argv, int start, int cmd_count);

#endif
