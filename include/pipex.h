/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 13:01:51 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/09 00:02:14 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include "libft.h"

typedef enum e_redir_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
}	t_redir;

typedef struct s_command
{
	char	**argv;
	t_list	*redirs;
}	t_command;

typedef struct s_pipeline
{
	t_list	*commands;
	int		cmd_count;
	int		**pipes;
	pid_t	*pids;
}	t_pipeline;

t_command	*command_create(char *cmd_str);
void		command_destroy(void *cmd);
int			command_add_redir(t_command *cmd, t_redir_type type, char *file);
t_redir		*redir_create(t_redir_type type, char *file);
void		redir_destroy(void *redir);
int			handle_redirs(t_list *redirs);
t_pipeline	*pipeline_create(void);
void		pipeline_destroy(t_pipeline *pipeline);
int			pipeline_add_command(t_pipeline *p, t_command *cmd);
int			pipeline_execute(t_pipeline *p, char **envp);
int			execute_command(t_command *cmd, char **envp);
char		*find_executable(char *cmd, char **envp);
void		error_exit_code(int code, char *msg, char *target);
void		safe_close(int *fd);
void		close_all_pipes(t_pipeline *p);
void		cleanup_pipeline(t_pipeline *p);
char		**parse_args(const char *input);

#endif
