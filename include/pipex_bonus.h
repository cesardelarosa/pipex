/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:17:00 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/02/20 10:49:29 by cde-la-r         ###   ########.fr       */
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
}			t_pipex;

void		execute_command(char *cmd, char **envp);
int			execute_pipeline(t_pipex *pipex);
void		parse_input(int argc, char **argv, char **envp, t_pipex *pipex);

#endif
