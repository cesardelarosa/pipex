/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_execute.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:28:19 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/08 13:00:14 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	create_pipes(t_pipeline *p, t_context *ctx)
{
	int	i;

	p->pipes = ft_calloc(p->cmd_count - 1, sizeof(int *));
	if (!p->pipes)
		error_exit_code(1, "malloc failed", NULL, ctx);
	i = 0;
	while (i < p->cmd_count - 1)
	{
		p->pipes[i] = ft_calloc(2, sizeof(int));
		if (!p->pipes[i] || pipe(p->pipes[i]) < 0)
		{
			while (i > 0)
			{
				i--;
				free(p->pipes[i]);
			}
			free(p->pipes);
			error_exit_code(1, "pipe creation failed", NULL, ctx);
		}
		i++;
	}
	return (1);
}

static void	setup_child_pipes(int cmd_idx, t_pipeline *p)
{
	int	i;

	i = 0;
	while (i < p->cmd_count - 1)
	{
		if (cmd_idx > 0 && i == cmd_idx - 1)
			dup2(p->pipes[i][0], STDIN_FILENO);
		if (cmd_idx < p->cmd_count - 1 && i == cmd_idx)
			dup2(p->pipes[i][1], STDOUT_FILENO);
		close(p->pipes[i][0]);
		close(p->pipes[i][1]);
		i++;
	}
}

static int	fork_command(t_command *cmd, int cmd_idx, t_pipeline *p,
			t_context *ctx)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		setup_child_pipes(cmd_idx, p);
		if (handle_redirs(cmd->redirs, ctx) < 0)
			exit(ctx->exit_code);
		execute_command(cmd, ctx);
	}
	return (pid);
}

static int	wait_for_children(t_pipeline *p, t_context *ctx)
{
	int	status;
	int	i;

	i = 0;
	while (i < p->cmd_count)
	{
		if (waitpid(p->pids[i], &status, 0) > 0)
		{
			if (WIFEXITED(status))
				ctx->exit_code = WEXITSTATUS(status);
		}
		i++;
	}
	return (ctx->exit_code);
}

int	pipeline_execute(t_pipeline *p, t_context *ctx)
{
	t_list	*current_cmd;
	int		cmd_idx;

	p->cmd_count = ft_lstsize(p->commands);
	if (p->cmd_count < 1)
		return (0);
	p->pids = ft_calloc(p->cmd_count, sizeof(pid_t));
	if (!p->pids)
		error_exit_code(1, "malloc failed", NULL, ctx);
	if (!create_pipes(p, ctx))
		return (0);
	current_cmd = p->commands;
	cmd_idx = 0;
	while (cmd_idx < p->cmd_count && current_cmd)
	{
		p->pids[cmd_idx] = fork_command(current_cmd->content, cmd_idx, p, ctx);
		if (p->pids[cmd_idx] < 0)
			break ;
		current_cmd = current_cmd->next;
		cmd_idx++;
	}
	close_all_pipes(p);
	return (wait_for_children(p, ctx));
}
