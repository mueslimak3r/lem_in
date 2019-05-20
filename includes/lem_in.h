/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/19 21:34:59 by alkozma           #+#    #+#             */
/*   Updated: 2019/05/19 22:38:38 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# define MAX_ROOMS	5000

# include 			"../libft/libft.h"
# include 			<unistd.h>

typedef struct		s_hashes
{
	char			*data[MAX_ROOMS];
	uint16_t		matrix[MAX_ROOMS][MAX_ROOMS];
	size_t			size;
}					t_hashes;

typedef struct		s_map
{
	t_hashes		hash_info;
	uint16_t		start;
	uint16_t		end;
	int				ants;
}					t_map;

int					read_data(t_map *in);
uint16_t			hash_id(char *id);

#endif
