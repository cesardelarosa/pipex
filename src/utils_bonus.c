#include "struct_bonus.h"

void	execute_command(char *cmd, char **envp)
{
	char	*args[4];

	args[0] = "/bin/sh";
	args[1] = "-c";
	args[2] = cmd;
	args[3] = NULL;
	if (execve("/bin/sh", args, envp) == -1)
		ft_handle_errors("pipex", "command not found", cmd, 127);
}

void	handle_child_process(t_pipex *pipex, int i, int in_fd, int *pipe_fd)
{
	if (dup2(in_fd, STDIN_FILENO) == -1)
		ft_handle_errors("pipex", "dup2 failed", "input", 1);
	close(in_fd);
	if (i < pipex->cmd_count - 1)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		close(pipe_fd[0]);
	}
	else
	{
		dup2(pipex->outfile_fd, STDOUT_FILENO);
		close(pipex->outfile_fd);
	}
	execute_command(pipex->cmds[i], pipex->envp);
}

void	handle_parent_process(pid_t pid, int *in_fd, int *pipe_fd, int i)
{
	int	status;

	waitpid(pid, &status, 0);
	if (i >= 0)
		close(*in_fd);
	if (*pipe_fd != -1)
	{
		*in_fd = pipe_fd[0];
		close(pipe_fd[1]);
	}
}

static void	read_here_doc(const char *delimiter, int write_fd)
{
	char	*line;

	while (1)
	{
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		line[ft_strcspn(line, "\n")] = '\0';
		if (ft_strcmp(line, (char *)delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
}

void	setup_here_doc(t_pipex *pipex)
{
	int	here_doc_pipe[2];

	if (pipe(here_doc_pipe) == -1)
		ft_handle_errors("pipex", "pipe creation failed", NULL, 1);
	read_here_doc(pipex->limiter, here_doc_pipe[1]);
	pipex->infile_fd = here_doc_pipe[0];
	close(here_doc_pipe[1]);
}
