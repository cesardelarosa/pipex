/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:21:31 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/07 17:27:42 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"
#include <stdio.h>
#include <stdlib.h>

static int	validate_arguments(int argc, char **argv)
{
	if (argc != 5)
	{
		ft_printf("Usage: %s infile \"cmd1\" \"cmd2\" outfile\n", argv[0]);
		return (0);
	}
	return (1);
}

static t_command	*create_command(char *cmd_str)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = parse_args(cmd_str);
	if (!cmd->argv)
	{
		free(cmd);
		return (NULL);
	}
	cmd->redir = NULL;
	return (cmd);
}

static void	free_command(void *content)
{
	t_command	*cmd;

	cmd = (t_command *)content;
	if (cmd)
	{
		if (cmd->redir)
			free(cmd->redir);
		if (cmd->argv)
			ft_free_split(cmd->argv);
		free(cmd);
	}
}

static t_pipeline	*init_pipeline(void)
{
	t_pipeline	*pipeline;

	pipeline = malloc(sizeof(t_pipeline));
	if (!pipeline)
		return (perror("malloc"), NULL);
	pipeline->cmd_list = NULL;
	pipeline->n = 0;
	return (pipeline);
}

static t_command	*create_redir_command(char *cmd_str, int redir_type,
		char *file)
{
	t_command	*cmd;
	t_redir		*redir;

	cmd = create_command(cmd_str);
	if (!cmd)
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
	{
		free_command(cmd);
		return (NULL);
	}
	redir->type = redir_type;
	redir->file = file;
	cmd->redir = redir;
	return (cmd);
}

static int	add_command_to_pipeline(t_pipeline *pipeline, char *cmd_str,
		int redir_type, char *file)
{
	t_list		*new_node;
	t_command	*cmd;

	cmd = create_redir_command(cmd_str, redir_type, file);
	if (!cmd)
		return (0);
	new_node = ft_lstnew(cmd);
	if (!new_node)
	{
		free_command(cmd);
		return (0);
	}
	ft_lstadd_back(&pipeline->cmd_list, new_node);
	pipeline->n++;
	return (1);
}

static void	destroy_pipeline(t_pipeline *pipeline)
{
	if (pipeline)
	{
		ft_lstclear(&pipeline->cmd_list, free_command);
		free(pipeline);
	}
}

static t_pipeline	*setup_pipeline(char **argv)
{
	t_pipeline	*pipeline;

	pipeline = init_pipeline();
	if (!pipeline)
		return (NULL);
	if (!add_command_to_pipeline(pipeline, argv[2], REDIR_INPUT, argv[1])
		|| !add_command_to_pipeline(pipeline, argv[3], REDIR_OUTPUT, argv[4]))
	{
		destroy_pipeline(pipeline);
		return (NULL);
	}
	return (pipeline);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipeline	*pipeline;
	int			exit_code;

	if (!validate_arguments(argc, argv))
		return (1);
	pipeline = setup_pipeline(argv);
	if (!pipeline)
		return (1);
	exit_code = execute_pipeline(pipeline, envp);
	destroy_pipeline(pipeline);
	return (exit_code);
}
