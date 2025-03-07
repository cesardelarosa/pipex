/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 13:51:46 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/07 19:32:48 by cesi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"

# define USAGE "./pipex <infile> <cmd1> <cmd2> <outfile>"

typedef struct s_list_n
{
	t_list			*list;
	int				n;
}					t_list_n;

typedef enum e_redir_type
{
	REDIR_INPUT,
	REDIR_OUTPUT
}					t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
}					t_redir;

typedef struct s_command
{
	char			**argv;
	t_list_n		*redir;
}					t_command;

typedef	struct	s_shell
{
	char	*name;
	char	**envp;
	char	*file;
	char	*str_error;
	int		exit;
}	t_shell;

int					execute_command(t_command *command, char **envp);
int					execute_pipeline(t_pipeline *pipeline, char **envp);
char				**parse_args(const char *input);
int					handle_redir(t_redir *redir);
void				ft_print_error(const char *s1, const char *s2, const char *s3);

#endif
