/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:17:00 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/02/19 19:47:44 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# define USAGE "./pipex [\"here_doc\" <delimiter>]/[<infile>] <cmds> <outfile>"

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "libft.h"

typedef struct s_pipex
{
	int		cmd_count;
	char	**cmds;
	char	**envp;
	int		infile_fd;
	int		outfile_fd;
	int		here_doc;
	char	*limiter;
}	t_pipex;

t_pipex	*init_pipex(void);
void	free_pipex(t_pipex *pipex);

void	setup_here_doc(t_pipex *pipex);

void	execute_command(char *cmd, char **envp);

t_pipex	*parse_input(int argc, char **argv, char **envp);

#endif
