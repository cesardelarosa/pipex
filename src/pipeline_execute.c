/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_execute.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:28:19 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/09 02:18:30 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	create_pipes(t_pipeline *p)
{
	int	i;

	p->pipes = ft_calloc(p->cmd_count - 1, sizeof(int *));
	if (!p->pipes)
		error_exit_code(1, "malloc failed", NULL);
	i = 0;
	while (i < p->cmd_count - 1)
	{
		p->pipes[i] = ft_calloc(2, sizeof(int));
		if (!p->pipes[i] || pipe(p->pipes[i]) < 0)
		{
			while (i-- > 0)
				free(p->pipes[i]);
			free(p->pipes);
			error_exit_code(1, "pipe creation failed", NULL);
		}
		i++;
	}
	return (1);
}

static void	setup_child_pipes(int index, t_pipeline *p)
{
	int	i;

	i = 0;
	while (i < p->cmd_count - 1)
	{
		if (i == index - 1 && dup2(p->pipes[i][0], STDIN_FILENO) == -1)
			error_exit_code(1, strerror(errno), "dup2");
		if (i == index && dup2(p->pipes[i][1], STDOUT_FILENO) == -1)
			error_exit_code(1, strerror(errno), "dup2");
		safe_close(&p->pipes[i][0]);
		safe_close(&p->pipes[i][1]);
		i++;
	}
}

static int	fork_command(t_command *cmd, int index, t_pipeline *p, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		setup_child_pipes(index, p);
		handle_redirs(cmd->redirs);
		execute_command(cmd, envp);
		error_exit_code(1, strerror(errno), "execute_command");
	}
	return (pid);
}

static int	wait_for_children(t_pipeline *p)
{
	int	exit;
	int	status;
	int	i;

	i = 0;
	while (i < p->cmd_count)
	{
		if (waitpid(p->pids[i], &status, 0) > 0)
		{
			if (WIFEXITED(status))
				exit = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (exit);
}

int	pipeline_execute(t_pipeline *p, char **envp)
{
	t_list	*current_cmd;
	int		index;

	p->cmd_count = ft_lstsize(p->commands);
	if (p->cmd_count < 1)
		return (0);
	p->pids = ft_calloc(p->cmd_count, sizeof(pid_t));
	if (!p->pids)
		error_exit_code(1, "malloc failed", NULL);
	if (!create_pipes(p))
		return (0);
	current_cmd = p->commands;
	index = 0;
	while (index < p->cmd_count && current_cmd)
	{
		p->pids[index] = fork_command(current_cmd->content, index, p, envp);
		if (p->pids[index] < 0)
			error_exit_code(1, "fork failed", NULL);
		current_cmd = current_cmd->next;
		index++;
	}
	close_all_pipes(p);
	return (wait_for_children(p));
}
