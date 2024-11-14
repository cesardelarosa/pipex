#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "struct_bonus.h"

void	setup_here_doc(t_pipex *pipex);
void	handle_child_process(t_pipex *pipex, int i, int in_fd, int *pipe_fd);
void	handle_parent_process(pid_t pid, int *in_fd, int *pipe_fd, int i);
void	execute_command(char *cmd, char **envp);

#endif
