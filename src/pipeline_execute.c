/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_execute.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:28:19 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/10 04:42:28 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "structs.h"
#include "execution.h"
#include "pipeline_utils.h"
#include "errors.h"

static int	create_pipes(t_pipeline *p)
{
	unsigned int	i;

	if (p->cmd_count < 1)
		return (0);
	p->pipes = ft_calloc(p->cmd_count - 1, sizeof(int *));
	if (!p->pipes)
		error_exit_code(1, "malloc failed", NULL, p);
	i = 0;
	while (i + 1 < p->cmd_count)
	{
		p->pipes[i] = ft_calloc(2, sizeof(int));
		if (!p->pipes[i] || pipe(p->pipes[i]) < 0)
		{
			while (i-- > 0)
			{
				close(p->pipes[i][0]);
				close(p->pipes[i][1]);
				free(p->pipes[i]);
			}
			free(p->pipes);
			error_exit_code(1, "pipe creation failed", NULL, p);
		}
		i++;
	}
	return (1);
}

static void	setup_child_pipes(unsigned int index, t_pipeline *p)
{
	unsigned int	i;

	i = 0;
	while (i + 1 < p->cmd_count)
	{
		if (i + 1 == index && dup2(p->pipes[i][0], STDIN_FILENO) == -1)
			error_exit_code(1, strerror(errno), "dup2", p);
		if (i == index && dup2(p->pipes[i][1], STDOUT_FILENO) == -1)
			error_exit_code(1, strerror(errno), "dup2", p);
		safe_close(&p->pipes[i][0]);
		safe_close(&p->pipes[i][1]);
		i++;
	}
}

static int	fork_command(t_command *cmd, unsigned int index, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		setup_child_pipes(index, cmd->p);
		execute_command(cmd, envp);
		error_exit_code(1, "execution failed", NULL, cmd->p);
	}
	return (pid);
}

static int	wait_for_children(t_pipeline *p)
{
	int				exit_status;
	int				status;
	pid_t			wpid;
	unsigned int	i;

	exit_status = 0;
	i = 0;
	while (i < p->cmd_count)
	{
		wpid = waitpid(p->pids[i], &status, 0);
		if (wpid < 0)
			error_exit_code(1, "waitpid failed", NULL, p);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_status = 128 + WTERMSIG(status);
		i++;
	}
	return (exit_status);
}

int	pipeline_execute(t_pipeline *p, char **envp)
{
	t_list			*current_cmd;
	unsigned int	index;

	p->pids = ft_calloc(p->cmd_count, sizeof(pid_t));
	if (!p->pids)
		error_exit_code(1, "malloc failed", NULL, p);
	if (!create_pipes(p))
		return (0);
	current_cmd = p->commands;
	index = 0;
	while (index < p->cmd_count && current_cmd)
	{
		p->pids[index] = fork_command(current_cmd->content, index, envp);
		if (p->pids[index] < 0)
			error_exit_code(1, "fork failed", NULL, p);
		current_cmd = current_cmd->next;
		index++;
	}
	close_all_pipes(p);
	return (wait_for_children(p));
}
