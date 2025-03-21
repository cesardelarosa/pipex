/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <code@cesardelarosa.xyz>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 12:14:35 by cde-la-r          #+#    #+#             */
/*   Updated: 2025/03/10 12:14:37 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPELINE_UTILS_H
# define PIPELINE_UTILS_H

# include "structs.h"

void		safe_close(int *fd);
void		close_all_pipes(t_pipeline *p);

#endif
