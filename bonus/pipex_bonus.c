/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:12:15 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/11/12 11:12:15 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	init_pipex(t_pipex *px, int argc, char **argv, char **envp)
{
	px->argc = argc;
	px->argv = argv;
	px->envp = envp;
	px->here_doc = 0;
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		px->here_doc = 1;
		px->cmd_count = argc - 4;
		px->infile = NULL;
		px->outfile = argv[argc - 1];
	}
	else
	{
		px->cmd_count = argc - 3;
		px->infile = argv[1];
		px->outfile = argv[argc - 1];
	}
}

void	execute_child(t_pipex *px, int *prev_fd, int *pipe_fd, int i)
{
	char	*cmd;
	int		*curr_prev_fd;
	int		*curr_pipe_fd;

	cmd = px->argv[i + 2 + px->here_doc];
	if (i == 0)
		curr_prev_fd = NULL;
	else
		curr_prev_fd = prev_fd;
	if (i != px->cmd_count - 1)
		curr_pipe_fd = pipe_fd;
	else
		curr_pipe_fd = NULL;
	child_process(px, cmd, curr_prev_fd, curr_pipe_fd);
}

void	fork_and_execute(t_pipex *px, int i, int prev_fd[2])
{
	int		pipe_fd[2];
	int		pid;
	int		is_last_cmd;

	is_last_cmd = (i == px->cmd_count - 1);
	if (!is_last_cmd)
	{
		if (pipe(pipe_fd) == -1)
			ft_handle_errors("pipex", "error creating pipe", NULL, 1);
	}
	pid = fork();
	if (pid == -1)
		ft_handle_errors("pipex", "error creating fork", NULL, 1);
	if (pid == 0)
	{
		if (is_last_cmd)
			execute_child(px, prev_fd, NULL, i);
		else
			execute_child(px, prev_fd, pipe_fd, i);
	}
	if (i != 0)
		close_pipes(prev_fd, NULL);
	if (!is_last_cmd)
		change_fd(prev_fd, pipe_fd);
}

void	create_pipes_and_forks(t_pipex *px)
{
	int	i;
	int	prev_fd[2];

	i = 0;
	while (i < px->cmd_count)
	{
		fork_and_execute(px, i, prev_fd);
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	px;

	if (argc < 5)
		ft_handle_errors("pipex", "invalid number of arguments", NULL, 1);
	init_pipex(&px, argc, argv, envp);
	create_pipes_and_forks(&px);
	wait_for_children(px.cmd_count);
	return (0);
}
