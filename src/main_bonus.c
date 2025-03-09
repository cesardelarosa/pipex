/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 18:07:08 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/08 19:33:37 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "struct_creation.h"
#include "execution.h"

#define HEREDOC_USAGE "./pipex here_doc LIMITER cmd [cmd ...] outfile"
#define NORMAL_USAGE "./pipex infile cmd [cmd ...] outfile"

static int	validate_args(int argc, char **argv)
{
	if (!argv || !argv[1])
	{
		ft_putstr_fd("Usage: " NORMAL_USAGE "\n", 2);
		exit(EXIT_FAILURE);
	}
	if (ft_strcmp(argv[1], "here_doc") == 0)
	{
		if (argc >= 6)
			return (1);
		ft_putstr_fd("Usage:" HEREDOC_USAGE "\n", 2);
		exit(EXIT_FAILURE);
	}
	else if (argc < 5)
	{
		ft_putstr_fd("Usage: " NORMAL_USAGE "\n", 2);
		exit(EXIT_FAILURE);
	}
	return (0);
}

static void	init_normal(t_pipeline *pipeline, int argc, char **argv)
{
	t_command	*cmd;
	int			i;

	cmd = command_create(argv[2]);
	command_add_redir(cmd, REDIR_INPUT, argv[1]);
	pipeline_add_command(pipeline, cmd);
	i = 3;
	while (i < argc - 1)
	{
		cmd = command_create(argv[i]);
		pipeline_add_command(pipeline, cmd);
		i++;
	}
	command_add_redir(cmd, REDIR_OUTPUT, argv[argc - 1]);
}

static void	init_heredoc(t_pipeline *pipeline, int argc, char **argv)
{
	t_command	*cmd;
	int			i;

	cmd = command_create(argv[3]);
	command_add_redir(cmd, REDIR_HEREDOC, argv[2]);
	pipeline_add_command(pipeline, cmd);
	i = 4;
	while (i < argc - 1)
	{
		cmd = command_create(argv[i]);
		pipeline_add_command(pipeline, cmd);
		i++;
	}
	command_add_redir(cmd, REDIR_APPEND, argv[argc - 1]);
}

int	main(int argc, char **argv, char **envp)
{
	int			is_heredoc;
	t_pipeline	*pipeline;
	int			exit;

	is_heredoc = validate_args(argc, argv);
	pipeline = pipeline_create();
	if (is_heredoc)
		init_heredoc(pipeline, argc, argv);
	else
		init_normal(pipeline, argc, argv);
	exit = pipeline_execute(pipeline, envp);
	pipeline_destroy(pipeline);
	return (exit);
}
