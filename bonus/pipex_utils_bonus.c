/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:11:39 by cde-la-r          #+#    #+#             */
/*   Updated: 2024/11/12 12:11:40 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	dup_with_error_check(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1)
	{
		close(oldfd);
		ft_handle_errors("pipex", "dup2 failed", NULL, 1);
	}
}

void	close_pipes(int *prev_fd, int *pipe_fd)
{
	if (prev_fd != NULL)
	{
		close(prev_fd[0]);
		close(prev_fd[1]);
	}
	if (pipe_fd != NULL)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

void	wait_for_children(int cmd_count)
{
	int	i;
	int	status;
	int	exit_code;
	int	child_exit_code;

	i = 0;
	exit_code = 0;
	while (i < cmd_count)
	{
		if (wait(&status) == -1)
			ft_handle_errors("pipex", "wait failed", NULL, 1);
		if (WIFEXITED(status))
		{
			child_exit_code = WEXITSTATUS(status);
			if (child_exit_code != 0)
				exit_code = child_exit_code;
		}
		i++;
	}
	exit(exit_code);
}

void	change_fd(int original_fd[2], int new_fd[2])
{
	original_fd[0] = new_fd[0];
	original_fd[1] = new_fd[1];
}
