/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/19 21:22:04 by alkozma           #+#    #+#             */
/*   Updated: 2019/05/20 05:05:42 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lem_in.h"

static int		handle_link(t_map *in, char *line)
{
	char		**tmp;
	uint16_t	h1;
	uint16_t	h2;
	int			i;
	int			n;

	tmp = ft_strsplit(line, '-');
	if (!tmp[1])
		return (0);
	h1 = hash_id(tmp[0]);
	h2 = hash_id(tmp[1]);
	i = 0;
	n = 0;
	while (in->hash_info.matrix[h1][i])
		i++;
	while (in->hash_info.matrix[h2][n])
		n++;
	in->hash_info.matrix[h1][i] = h2;
	in->hash_info.matrix[h2][n] = h1;
	return (1);
}

static int		handle_room(t_map *in, char *line, int stat)
{
	char		**tmp;
	int			i;
	uint16_t	hash;

	tmp = ft_strsplit(line, ' ');
	i = 0;
	if (!tmp[1] || ++in->hash_info.size > MAX_ROOMS)
		return (0);
	hash = hash_id(tmp[0]);
	if (in->hash_info.data[hash])
		ft_printf("COLLISION\n");
	in->hash_info.data[hash] = ft_strdup(tmp[0]);
	if (stat == 1 || stat == -1)
		(stat == 1) ? (in->start = hash) : (in->end = hash);
	while (tmp[i])
		free(tmp[i++]);
	free(tmp);
	return (1);
}

int			read_data(t_map *in)
{
	char	*line;
	//char	**tmp;
	int		stat;

	stat = 0;
	while (get_next_line(0, &line) > 0)
	{
		ft_printf("%s\n", line);
		if (line[0] == '#')
		{
			stat = ft_strcmp(line, "##start") == 0 ? 1 : stat;
			stat = ft_strcmp(line, "##end") == 0 ? -1 : stat;
		}
		else
		{
			if (in->ants == 0)
				in->ants = ft_atoi(line);
			if (ft_strchr(line, ' '))
				handle_room(in, line, stat);
			else if (ft_strchr(line, '-'))
				handle_link(in, line);
			if (stat)
				stat = 0;
		}
	}
	return (1);
}

uint16_t	hash_id(char *s)
{
    unsigned long   h = 0, high;
    while ( *s )
    {
        h = ( h << 4 ) + *s++;
        if (( high = h & 0xF0000000 ))
            h ^= high >> 24;
        h &= ~high;
    }
    return (h % MAX_ROOMS);
}
