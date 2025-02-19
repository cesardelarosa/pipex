/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:17:00 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/02/19 21:44:38 by cde-la-r         ###   ########.fr       */
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
	char	*limiter;
}	t_pipex;

void	execute_command(char *cmd, char **envp);
void	parse_input(int argc, char **argv, char **envp, t_pipex *pipex);

#endif
