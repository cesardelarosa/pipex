/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 13:02:12 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/09 14:16:49 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLE_REDIR_BONUS_H
# define HANDLE_REDIR_BONUS_H

# include "structs.h"

int	handle_redir_in(t_redir *redir);
int	handle_redir_out(t_redir *redir);
int	handle_redir_append(t_redir *redir);
int	handle_heredoc(t_redir *redir);

#endif
