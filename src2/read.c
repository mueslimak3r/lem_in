/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 02:13:15 by alkozma           #+#    #+#             */
/*   Updated: 2019/07/29 22:59:21 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lem_in.h"

uint16_t	hash_id(char *s)
{
	unsigned long	h;
	unsigned long	high;

	h = 0;
	while (*s)
	{
		h = (h << 4) + *s++;
		if ((high = h & 0xF0000000))
			h ^= high >> 24;
		h &= ~high;
	}
	return (h);
}

/*
** handle_link:
**    1. Splits the line by '-' (assumed "id1-id2" format)
**    2. Hashes both room ids.
**    3. Allocates new t_room structs.
**    4. Assigns them to the head of the "links" element of each
**       other's room.
**       a) The "links" pointer is a linked list of t_room pointers.
*/

int		handle_link(t_map *in, char *line)
{
	char		**tmp;
	uint16_t	h1;
	uint16_t	h2;
	t_room		*a;
	t_room		*b;

	tmp = ft_strsplit(line, '-');
	if (!tmp[1])
		return (0);
	h1 = hash_id(tmp[0]);
	h2 = hash_id(tmp[1]);
	a = malloc(sizeof(t_room));
	b = malloc(sizeof(t_room));
	a->hash = h2;
	b->hash = h1;
	a->prev = in->hash_info.list[h1 % 5000]->links;
	b->prev = in->hash_info.list[h2 % 5000]->links;
	in->hash_info.list[h1 % 5000]->links = a;
	in->hash_info.list[h2 % 5000]->links = b;
	return (1);
}

/*
** handle_room:
**    1. Splits the line by spaces (assumed "id x y" format)
**    2. Creates a hash of the room's id.
**    3. Checks to see if there is already data stored in this
**       hash's spot. If so, move to the last element of the
**       slot's data.
**    4. Allocates a new t_room pointer, setting it's appropriate
**       values.
**    5. If stat is set, assign the hash to the start or end
**       hash reference in the passed t_map pointer.
**    6. Assign the newly allocated room pointer to the hash
**       table.
*/

int		handle_room(t_map *in, char *line, int stat)
{
	char		**tmp;
	int			i;
	uint16_t	hash;
	t_room		*new;

	tmp = ft_strsplit(line, ' ');
	i = 0;
	if (!tmp[1])
		return (0);
	hash = hash_id(tmp[0]);
	new = malloc(sizeof(t_room));
	new->hash = hash;
	new->prev = in->hash_info.list[hash % 5000];
	new->links = NULL;
	new->from_start = NULL;
	new->from_end = NULL;
	new->id = ft_strdup(tmp[0]);
	in->start = stat == 1 ? hash : in->start;
	in->end = stat == -1 ? hash : in->end;
	in->hash_info.list[hash % 5000] = new;
	return (1);
}
