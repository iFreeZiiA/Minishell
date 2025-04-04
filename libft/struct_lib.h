/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_lib.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 18:28:49 by jjorda            #+#    #+#             */
/*   Updated: 2025/03/04 16:43:47 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_LIB_H
# define STRUCT_LIB_H

# include "../MLX42/include/MLX42/MLX42.h"

typedef struct s_list
{
	mlx_t			*mlx;
	void			*content;
	struct s_list	*next;
}	t_list;

#endif