/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:17:00 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/02/20 11:32:59 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# define USAGE "./pipex [\"here_doc\" <delimiter>]/[<infile>] <cmds> <outfile>"

# include "libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipex
{
	int		cmd_count;
	char	**cmds;
	char	**envp;
	int		infile_fd;
	int		outfile_fd;
	char	*limiter;
	pid_t	*pid_arr;
}			t_pipex;

void		execute_command(t_pipex *pipex, int i);
int			execute_pipeline(t_pipex *pipex);
void		parse_input(int argc, char **argv, char **envp, t_pipex *pipex);
void		free_and_exit(t_pipex *pipex, char *msg, char *cmd, int exit_code);
void		free_pipex(t_pipex *pipex);

#endif
