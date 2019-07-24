/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/19 21:34:59 by alkozma           #+#    #+#             */
/*   Updated: 2019/07/23 19:01:21 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# define MAX_ROOMS	10000
# define MAX_LINKS	100

# include "../libft/libft.h"
# include <unistd.h>

typedef struct		s_room
{
	uint16_t		hash;
	struct s_room	*prev;
	int				depth;
}					t_room;

typedef struct		s_path
{
	bool			is_last;
	size_t			len;
	struct s_room	*tail;
	struct s_path	*next;
	struct s_path	*prev;
}					t_path;

typedef struct		s_hashes
{
	char			*data[MAX_ROOMS];
	uint16_t		matrix[MAX_ROOMS][MAX_LINKS];
	size_t			size;
}					t_hashes;

typedef struct		s_map
{
	t_hashes		hash_info;
	uint16_t		start;
	uint16_t		end;
	int				ants;
	uint64_t		size;
}					t_map;

typedef struct		s_mypaths
{
	struct s_path	*paths;
	struct s_path	*removed;
	struct s_path	*complete;
}					t_mypaths;

typedef struct		s_sorted
{
	struct s_path	*paths;
	size_t			flow;
	size_t			length;
	struct s_hash	*hash;
	struct s_sorted	*next;
	struct s_sorted	*prev;
}					t_sorted;

typedef struct		s_hash
{
	uint16_t		*matrix;
	size_t			size;
}					t_hash;

int					read_data(t_map *in);
uint16_t			hash_id(char *id);
int					solve(t_map *map);
void				sort_complete(t_map *map, t_mypaths *p);
void        print_path(t_map *map, t_path *path);
int					solver(t_map *in, t_path **paths);
void				push_sorted(t_sorted **sorted, t_path *path);
void				hash_path(t_hash **hash, t_path *path, t_map *map);
void				check(t_sorted **sorted, t_path *path, t_hash *hash);

#endif
