/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:28:01 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/08 18:24:19 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	validate_args(int argc)
{
	if (argc == 5)
		return ;
	ft_putstr_fd("Usage: ./pipex <infile> <cmd1> <cmd2> <outfile>\n", 2);
	exit(EXIT_FAILURE);
}

static void	init_pipeline(t_pipeline *pipeline, char **argv)
{
	t_command		*cmd1;
	t_command		*cmd2;

	cmd1 = command_create(argv[2]);
	command_add_redir(cmd1, REDIR_INPUT, argv[1]);
	pipeline_add_command(pipeline, cmd1);
	cmd2 = command_create(argv[3]);
	command_add_redir(cmd2, REDIR_OUTPUT, argv[4]);
	pipeline_add_command(pipeline, cmd2);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipeline		*pipeline;
	int				exit;

	validate_args(argc);
	pipeline = pipeline_create();
	init_pipeline(pipeline, argv);
	exit = pipeline_execute(pipeline, envp);
	pipeline_destroy(pipeline);
	return (exit);
}
