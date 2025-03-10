/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 12:16:09 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/10 12:16:10 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "struct_creation.h"
#include "execution.h"
#include "errors.h"

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

static int	init_normal(t_pipeline *pipeline, int argc, char **argv)
{
	t_command	*cmd;
	int			i;

	cmd = command_create(argv[2]);
	if (!cmd || !command_add_redir(cmd, REDIR_INPUT, argv[1])
		|| !pipeline_add_command(pipeline, cmd))
		return (0);
	i = 3;
	while (i < argc - 1)
	{
		cmd = command_create(argv[i]);
		if (!cmd || !pipeline_add_command(pipeline, cmd))
			return (0);
		i++;
	}
	if (!command_add_redir(cmd, REDIR_OUTPUT, argv[argc - 1]))
		return (0);
	return (1);
}

static int	init_heredoc(t_pipeline *pipeline, int argc, char **argv)
{
	t_command	*cmd;
	int			i;

	cmd = command_create(argv[3]);
	if (!cmd || !command_add_redir(cmd, REDIR_HEREDOC, argv[2])
		|| !pipeline_add_command(pipeline, cmd))
		return (0);
	i = 4;
	while (i < argc - 1)
	{
		cmd = command_create(argv[i]);
		if (!cmd || !pipeline_add_command(pipeline, cmd))
			return (0);
		i++;
	}
	if (!command_add_redir(cmd, REDIR_APPEND, argv[argc - 1]))
		return (0);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	int			is_heredoc;
	t_pipeline	*pipeline;
	int			exit;
	int			init_ok;

	is_heredoc = validate_args(argc, argv);
	pipeline = pipeline_create();
	if (is_heredoc)
		init_ok = init_heredoc(pipeline, argc, argv);
	else
		init_ok = init_normal(pipeline, argc, argv);
	if (!init_ok)
		error_exit_code(1, "allocation error", NULL, pipeline);
	exit = pipeline_execute(pipeline, envp);
	pipeline_destroy(pipeline);
	return (exit);
}
