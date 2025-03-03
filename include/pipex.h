/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 13:51:46 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/04 00:14:04 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define USAGE "./pipex <infile> <cmd1> <cmd2> <outfile>"

void	execute_command(char *cmd, char **envp);
char	**parse_args(const char *input);

#endif
