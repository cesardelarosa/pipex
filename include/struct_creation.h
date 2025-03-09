/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_creation.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 13:01:51 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/09 15:15:26 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_CREATION_H
# define STRUCT_CREATION_H

# include "structs.h"

char		**parse_args(const char *input);

t_command	*command_create(char *cmd_str);
void		command_destroy(void *cmd);
int			command_add_redir(t_command *cmd, t_redir_type type, char *file);

t_pipeline	*pipeline_create(void);
void		pipeline_destroy(t_pipeline *pipeline);
int			pipeline_add_command(t_pipeline *p, t_command *cmd);

#endif
