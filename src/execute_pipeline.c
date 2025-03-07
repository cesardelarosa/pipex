/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:19:19 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/07 14:30:19 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static int	create_pipes(t_pipeline *pipeline, int ***pipes)
{
	int	i;

	if (pipeline->n <= 1)
		return (*pipes = NULL, 0);
	*pipes = malloc(sizeof(int *) * (pipeline->n - 1));
	if (!*pipes)
		return (1);
	i = -1;
	while (++i < pipeline->n - 1)
	{
		(*pipes)[i] = malloc(sizeof(int) * 2);
		if (!(*pipes)[i] || pipe((*pipes)[i]) < 0)
		{
			while (i-- >= 0)
				free((*pipes)[i]);
			free(*pipes);
			return (1);
		}
	}
	return (0);
}

static void	configure_child_pipes(int i, t_pipeline *p, int **pipes)
{
	int	j;

	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < p->n - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	j = -1;
	while (++j < p->n - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
	}
}

static void	execute_child(int i, t_pipeline *p, int **pipes, char **envp)
{
	t_command	*cmd;
	t_list		*node;

	node = ft_lstget(p->cmd_list, i);
	if (!node)
		exit(1);
	cmd = (t_command *)node->content;
	if (pipes)
		configure_child_pipes(i, p, pipes);
	if (cmd->redir)
		handle_redir(cmd->redir);
	exit(execute_command(cmd, envp));
}

static int	fork_processes(t_pipeline *p, pid_t *pids, int **pipes, char **envp)
{
	int	i;

	i = -1;
	while (++i < p->n)
	{
		pids[i] = fork();
		if (pids[i] < 0)
			return (i = -1, 1);
		if (pids[i] == 0)
			execute_child(i, p, pipes, envp);
	}
	return (0);
}

static void	cleanup_resources(int **pipes, t_pipeline *p)
{
	int	i;

	if (pipes)
	{
		i = -1;
		while (++i < p->n - 1)
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			free(pipes[i]);
		}
		free(pipes);
	}
}

static int	wait_children(pid_t *pids, t_pipeline *p)
{
	int	status;
	int	exit_code;
	int	i;

	exit_code = 0;
	i = -1;
	while (++i < p->n)
	{
		if (waitpid(pids[i], &status, 0) > 0 && WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
	}
	return (exit_code);
}

int	execute_pipeline(t_pipeline *pipeline, char **envp)
{
	pid_t	*pids;
	int		**pipes;
	int		exit_code;

	pipeline->n = ft_lstsize(pipeline->cmd_list);
	if (pipeline->n < 1)
		return (0);
	pids = malloc(sizeof(pid_t) * pipeline->n);
	if (!pids)
		return (1);
	if (create_pipes(pipeline, &pipes)
		|| fork_processes(pipeline, pids, pipes, envp))
	{
		cleanup_resources(pipes, pipeline);
		free(pids);
		return (1);
	}
	cleanup_resources(pipes, pipeline);
	exit_code = wait_children(pids, pipeline);
	free(pids);
	return (exit_code);
}
