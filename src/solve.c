/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/07 23:20:43 by alkozma           #+#    #+#             */
/*   Updated: 2019/07/26 02:32:51 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lem_in.h"

// pull_path

void        print_path(t_map *map, t_path *path)
{
    t_room  *rooms;

    rooms = path->tail;
    while (rooms)
    {
        ft_printf("%s->", map->hash_info.data[rooms->hash]);
        rooms = rooms->prev;
    }
    ft_printf("\n");
}

void        print_paths(t_map *map, t_path *paths, char *name)
{
    t_path  *tmp;

    tmp = paths;
    ft_printf("%s:\n", name);
    while (tmp)
    {
        print_path(map, tmp);
        if (tmp->next == paths)
            break ;
        tmp = tmp->next;
    }
}

void        count_nodes(t_path *paths)
{
    int i = 0;

    while (paths)
    {
        i++;
        if (paths->is_last || paths->next == paths)
            break ;
        paths = paths->next;
    }
    ft_printf("# of paths: %d\n", i);
}

void        add_room(t_path *new_path, t_room *old, uint16_t new_hash)
{
    t_room  *new;

    if (!(new = ft_memalloc(sizeof(t_room))))
        return ;
    new->hash = new_hash;
    new->prev = old;
    new_path->tail = new;
}

void        push_path(t_path **paths, t_room *old, uint16_t new_hash)
{
    t_path  *new;

    if (!(new = ft_memalloc(sizeof(t_path))))
        return ;
    add_room(new, old, new_hash);
    new->next = new;
    new->prev = new;
    new->is_last = true;
    if (*paths)
    {
        if ((*paths)->prev == *paths)
        {
            //ft_printf("added to size one list\n");
            new->prev = *paths;
            new->next = *paths;
            (*paths)->next = new;
            (*paths)->prev = new;
            (*paths)->is_last = false;
        }
        else
        {
            //ft_printf("added to list\n");
            //ft_printf("paths: %p pathslast: %p pathsnext: %p\n", *paths, (*paths)->prev, (*paths)->next);
            *paths = (*paths)->prev;
            (*paths)->is_last = false;
            new->next = (*paths)->next;
            new->prev = *paths;
            (*paths)->next->prev = new;
            (*paths)->next = new;
            *paths = new->next;
            //ft_printf("paths: %p pathslast: %p pathsnext: %p\n", *paths, (*paths)->prev, (*paths)->next);
        }
    }
    else
    {
        //ft_printf("added to empty list\n");
        *paths = new;
    }
    (*paths)->len += 1;
}

void        switch_lists(t_path **a, t_path **b)
{
    t_path  *temp;

    temp = *a;
    if (*a != (*a)->next)
    {
        //ft_printf("hello!\n");
        //ft_printf("paths: %p pathslast: %p pathsnext: %p\n", *a, (*a)->prev, (*a)->next);
        if ((*a)->is_last)
        {
            //ft_printf("islast = true\n");
            (*a)->prev->is_last = true;
        }
        *a = (*a)->next;
        (*a)->prev = temp->prev;
        temp->prev->next = *a;
        //ft_printf("paths: %p pathslast: %p pathsnext: %p\n", *a, (*a)->prev, (*a)->next);
    }
    else
    {
        ft_printf("set *a to NULL\n");
        *a = NULL;
    }
    temp->next = *b ? *b : temp;
    temp->prev = temp;
    if (*b)
    {
        temp->prev = (*b)->prev;
        (*b)->prev->next = temp;
        (*b)->prev = temp;
    }
    temp->is_last = (*b) ? false : true;
    *b = temp;
    if (*a)
        (*a)->len -= 1;
    (*b)->len += 1;
    //ft_printf("removed %p", *b);
    //ft_printf(" a: %p\n", *a);
}

bool        check_overlaps(t_room *tail, uint16_t hash)
{
    while (tail)
    {
        if (tail->hash == hash)
        {
            //ft_printf("overlaps!\n");
            return (true);
        }
        tail = tail->prev;
    }
    return (false);
}

void        mod_path(t_map *map, t_mypaths *p, int mode)
{
    int     i;

    i = 0;
    if (((*p).paths->tail->hash == map->end && !mode) || ((*p).paths->tail->hash == map->start && mode))
    {
        //ft_printf("path complete!\n");
        //print_path(map, (*p).paths);
        switch_lists(&(*p).paths, &(*p).complete);
        return ;
    }
    while (map->hash_info.matrix[(*p).paths->tail->hash][i])
    {
        //ft_printf("trying to add: %s\n", map->hash_info.data[map->hash_info.matrix[(*p).paths->tail->hash][i]]);
        if (!check_overlaps((*p).paths->tail, map->hash_info.matrix[(*p).paths->tail->hash][i]))
        {
            push_path(&(*p).paths, (*p).paths->tail, map->hash_info.matrix[(*p).paths->tail->hash][i]);
            //ft_printf("added: %s\n", map->hash_info.data[map->hash_info.matrix[(*p).paths->tail->hash][i]]);
           // print_paths(map, (*p).paths, "paths");
           map->size++;
        }
        i++;
    }
    switch_lists(&(*p).paths, &(*p).removed);
    if ((*p).removed)
    {
        free((*p).removed);
        (*p).removed = NULL;
        map->size--;
    }
}

int			get_depth(t_path **path)
{
	t_room	*tmp;
	int		ret;

	tmp = (*path)->tail;
	ret = 0;
	while (tmp)
	{
		tmp = tmp->prev;
		ret++;
	}
	return (ret);
}

/*void		clean_up_queue(t_mypaths *mypaths, t_map *in)
{
	t_path	*tmp1;
	t_room	*tmp2;
	t_room	*buf;
	t_path	*buf2;

	tmp1 = mypaths->complete;
	while (!tmp1->is_last)
	{
		tmp2 = tmp1->tail;
		while (tmp2 && tmp2->prev)
		{
			buf = tmp2;
			tmp2 = tmp2->prev;
			free(buf);
		}
		free(tmp2);
		buf2 = tmp1;
		tmp1 = tmp1->next;
		free(buf2);
	}
	ft_printf("HEY\n");
	free(mypaths);
	free(in);
}*/

int         solve(t_map *map)
{
    t_mypaths   p;
	t_mypaths	b;

    if (!map->start || !map->end)
        return (0);
    ft_memset(&p, 0, sizeof(t_mypaths));
    p.complete = NULL;
    p.paths = NULL;
    p.removed = NULL;
    ft_memset(&b, 0, sizeof(t_mypaths));
    b.complete = NULL;
    b.paths = NULL;
    b.removed = NULL;
    push_path(&b.paths, NULL, map->end);
    push_path(&p.paths, NULL, map->start);
    map->size = 1;
    while (p.paths)
    {
		//ft_printf("%d\n", ind_paths(p.paths->tail, p.complete, map));
		if (get_depth(&(p.paths)) > 8)
			break;
        mod_path(map, &p, 0);
    }
	while (b.paths)
	{
		if (get_depth(&(b.paths)) > 8)
			break;
		mod_path(map, &b, 1);
	}
	/*while (p.complete)
	{
		ft_printf("%d\n", ind_paths(p.complete->tail, p.complete, map));
		p.complete = p.complete->next;
	}*/
    count_nodes(p.paths);
    count_nodes(p.complete);
	count_nodes(b.paths);
	count_nodes(b.complete);
    print_paths(map, b.complete, "complete");
    print_paths(map, p.complete, "complete");
    sort_complete(map, &p);
	//clean_up_queue(&p, map);
    return (1);
}
