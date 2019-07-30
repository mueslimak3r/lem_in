/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 02:07:15 by alkozma           #+#    #+#             */
/*   Updated: 2019/07/30 00:16:02 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lem_in.h"

void	print_path(t_map *in, t_path *path)
{
	t_room	*rooms;
	
	if (!in)
		;
	rooms = path->tail;
	if (!rooms)
		ft_printf("%s\n", "NO ROOMS");
	while (rooms)
	{
		ft_printf("%s->", rooms->id);
		if (rooms->prev == path->tail || rooms->prev == rooms)
			break ;
		rooms = rooms->prev;
	}
	ft_printf("\n");
}

int		main(void)
{
	t_map	in;
	t_cluster_node	*tmp;
	
	ft_memset(&in, 0, sizeof(t_map));
	g_completed_paths = NULL;
	if (!read_data(&in))
		return (ft_printf("ERR\n"));
	solve(&in);
	ft_printf("HEY|\n");
	tmp = g_completed_paths;
	while (g_completed_paths)
	{
		print_path(&in, g_completed_paths->path);
		if (g_completed_paths->next == tmp)
			break ;
		g_completed_paths = g_completed_paths->next;
	}
	return (0);
}

/*
** read_data:
**   1. Reads a line off of standard input.
**   2. Prints it back out in accordance with desired final output.
**   3. Checks to see if it's a comment, start, or end command.
**      a) If it's a start or end command, it sets `stat` appropriately
**         for later processing.
**      b) If it isn't, ignore it.
**   4. Determines whether it is:
**      a) The first uncommented line (ant number)
**      b) A room definition string
**      c) A link definition string
*/

int		read_data(t_map *in)
{
	char	*line;
	t_room	*rooms;
	int		stat;
	
	rooms = NULL;
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
			if (ft_strchr(line, ' ') && !(handle_room(in, line, stat)))
				return (0);
			else if (ft_strchr(line, '-') && !(handle_link(in, line)))
				return (0);
		}
	}
	return (1);
}
