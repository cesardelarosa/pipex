/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:28:01 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/08 12:58:21 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	show_usage(char *prog_name)
{
	ft_putstr_fd("Usage: ", 2);
	ft_putstr_fd(prog_name, 2);
	ft_putstr_fd(" <infile> <cmd1> <cmd2> <outfile>\n", 2);
}

int	main(int argc, char **argv, char **envp)
{
	t_context		ctx;
	t_pipeline		*pipeline;
	t_command		*cmd1;
	t_command		*cmd2;

	if (argc != 5)
	{
		show_usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	ctx.prog_name = argv[0];
	ctx.envp = envp;
	ctx.exit_code = 0;
	ctx.open_fds = NULL;
	pipeline = pipeline_create();
	cmd1 = command_create(argv[2]);
	command_add_redir(cmd1, REDIR_INPUT, argv[1]);
	pipeline_add_command(pipeline, cmd1);
	cmd2 = command_create(argv[3]);
	command_add_redir(cmd2, REDIR_OUTPUT, argv[4]);
	pipeline_add_command(pipeline, cmd2);
	pipeline_execute(pipeline, &ctx);
	pipeline_destroy(pipeline);
	return (ctx.exit_code);
}
