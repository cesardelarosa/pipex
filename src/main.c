/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:28:01 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/10 11:28:19 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "struct_creation.h"
#include "execution.h"
#include "errors.h"

#define USAGE "./pipex infile cmd1 cmd2 outfile"

static void	validate_args(int argc)
{
	if (argc == 5)
		return ;
	ft_putstr_fd("Usage: " USAGE "\n", 2);
	exit(EXIT_FAILURE);
}

static int	init_pipeline(t_pipeline *pipeline, char **argv)
{
	t_command		*cmd1;
	t_command		*cmd2;

	cmd1 = command_create(argv[2]);
	if (!cmd1 || !command_add_redir(cmd1, REDIR_INPUT, argv[1])
		|| !pipeline_add_command(pipeline, cmd1))
		return (0);
	cmd2 = command_create(argv[3]);
	if (!cmd2 || !command_add_redir(cmd2, REDIR_OUTPUT, argv[4])
		|| !pipeline_add_command(pipeline, cmd2))
		return (0);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipeline		*pipeline;
	int				exit;

	validate_args(argc);
	pipeline = pipeline_create();
	if (!init_pipeline(pipeline, argv))
		error_exit_code(1, "allocation error", NULL, pipeline);
	exit = pipeline_execute(pipeline, envp);
	pipeline_destroy(pipeline);
	return (exit);
}
