#ifndef STRUCT_BONUS_H
# define STRUCT_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include "libft.h"

typedef struct s_pipex
{
	int		here_doc;
	char	*limiter;
	int		infile_fd;
	int		outfile_fd;
	char	**cmds;
	char	**envp;
	int		cmd_count;
}	t_pipex;

#endif
