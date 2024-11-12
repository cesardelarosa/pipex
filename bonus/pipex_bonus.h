/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:32:13 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/11/12 11:32:13 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "libft.h"

typedef struct s_pipex
{
	int		argc;
	char	**argv;
	char	**envp;
	int		cmd_count;
	int		here_doc;
	char	*infile;
	char	*outfile;
}	t_pipex;

void	dup_with_error_check(int oldfd, int newfd);
void	execute_command(char *cmd, char **envp);
void	child_process(t_pipex *px, char *cmd, int *prev_fd, int *pipe_fd);
void	setup_infile(t_pipex *px);
void	setup_outfile(t_pipex *px);
void	handle_here_doc(t_pipex *px);
void	close_pipes(int *prev_fd, int *pipe_fd);
void	change_fd(int original_fd[2], int new_fd[2]);
void	wait_for_children(int cmd_count);
void	init_pipex(t_pipex *px, int argc, char **argv, char **envp);
void	create_pipes_and_forks(t_pipex *px);
void	fork_and_execute(t_pipex *px, int i, int prev_fd[2]);

#endif
