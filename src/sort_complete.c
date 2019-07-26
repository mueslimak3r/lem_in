#include "../includes/lem_in.h"

bool    check_against(t_path *other, t_hash *hash)
{
    t_room *rooms;

    rooms = other->tail;
    while (rooms)
    {
        if (rooms->hash == hash->matrix[rooms->hash])
            return (false);
        rooms = rooms->prev;
    }
	return (true);
}

void        push_sorted(t_path **list, t_path *path)
{
    t_path *new;

    if (!path)
        return ;
    new = ft_memalloc(sizeof(t_path));
    new->tail = path->tail;
    new->next = new;
    new->prev = new;
    if (*list)
    {
        new->next = *list;
        (*list)->prev->next = new;
        new->prev = (*list)->prev;
        (*list)->prev = new;
    }
    *list = new;
    (*list)->len = path->len;
}

void        new_sorted(t_path *finished, t_path *toadd, t_sorted **sorted, t_map *map)
{
    t_path *f;

    f = finished;
    if (!*sorted)
    {
        *sorted = ft_memalloc(sizeof(t_sorted));
        (*sorted)->paths = NULL;
        (*sorted)->next = NULL;
		(*sorted)->hash = NULL;
		(*sorted)->prev = NULL;
    }
    while (f)
    {
        hash_path(&((*sorted)->hash), f, map);
        push_sorted(&((*sorted)->comp), f);
        if (f->next == finished)
            break ;
        f = f->next;
    }
    push_sorted(&((*sorted)->comp), toadd);
}

void        check(t_sorted **sorted, t_path *path, t_hash *hash)
{
    t_path      *other_path;
    t_sorted    *new;

    new = NULL;
    other_path = path->next;
    while (other_path != path)
    {
        if (check_against(other_path, hash))
            push_sorted(&(new->paths), other_path);
        other_path = other_path->next;
    }
    if (*sorted)
        new->next = *sorted;
    *sorted = new;
}

void    hash_path(t_hash **hash, t_path *path, t_map *map)
{
    t_room  *rooms;

    if (!*hash)
        *hash = ft_memalloc(sizeof(t_hash));
    if (!*hash)
        return ;
    (*hash)->matrix = ft_memalloc((sizeof(u_int16_t) * MAX_ROOMS) + 1);
    rooms = path->tail;
    while (rooms)
    {
        if (rooms->hash != map->start && rooms->hash != map->end)
            (*hash)->matrix[rooms->hash] = rooms->hash;
        rooms = rooms->prev;
    }
}

void    print_sorted(t_sorted *list, t_map *map)
{
    t_path *path;
    while (list)
    {
        path = list->paths;
        ft_printf("cluster %p:\n", list);
        while (path)
        {
            print_path(map, path);
            if (path->next == list->paths)
                break ;
            path = path->next;
        }
        ft_printf("\nother stuff:\n");
		path = list->paths;
		ft_printf("%d max flow\n", list->flow);
        list = list->next;
    }
}

void    push_cluster(t_sorted *in, t_sorted **list)
{
    in->next = *list;
    *list = in;
}

void    find_em(t_sorted **list, t_map *map, t_path *first)
{
    t_path      *path;
    t_sorted    *sorted_paths;
    t_sorted    *clusters;

    clusters = NULL;
    sorted_paths = NULL;
    path = first;
    while (path)
    {
        ft_printf("yo!\n");
        new_sorted(NULL, path, &sorted_paths, map);
        check(&sorted_paths, path, sorted_paths->hash);
        push_cluster(sorted_paths, &clusters);
        if (path->next == first)
            break ;
        path = path->next;
    }
    //add_subs(list, sorted_paths);
    //print_sorted(sorted_paths, map);
    *list = solver(map, clusters);
}

void    sort_complete(t_map *map, t_mypaths *p)
{
    t_sorted *paths;

    paths = NULL;
    print_paths(map, p->complete, "test print");
    find_em(&paths, map, p->complete);
}
