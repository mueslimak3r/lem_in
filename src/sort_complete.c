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

void        push_sorted(t_sorted **sorted, t_path *path)
{
    t_path *new;

    if (!*sorted)
    {
        *sorted = ft_memalloc(sizeof(t_sorted));
        (*sorted)->paths = NULL;
        (*sorted)->next = NULL;
    }
    new = ft_memalloc(sizeof(t_path));
    new->tail = path->tail;
    new->next = NULL;
    new->prev = new;
    if ((*sorted)->paths)
    {
        new->next = (*sorted)->paths;
        (*sorted)->paths->prev->next = new;
        new->prev = (*sorted)->paths->prev;
        (*sorted)->paths->prev = new;
    }
    (*sorted)->paths = new;
    (*sorted)->paths->len = path->len;
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
            push_sorted(&new, other_path);
        other_path = other_path->next;
    }
    push_sorted(&new, path);
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
        ft_printf("\n");
        list = list->next;
    }
}

void    find_em(t_sorted **list, t_map *map, t_path *first)
{
    t_path      *path;
    t_hash      *hash;
    t_sorted    *sorted_paths;

    sorted_paths = NULL;
    hash = NULL;
    path = first;
    while (path)
    {
        hash_path(&hash, path, map);
        check(&sorted_paths, path, hash);
        free(hash->matrix);
        free(hash);
        hash = NULL;
        if (path->next == first)
        {
            break ;
        }
        path = path->next;
    }
    //add_subs(list, sorted_paths);
    //print_sorted(sorted_paths, map);
    if (list)
        ;
}

void    sort_complete(t_map *map, t_mypaths *p)
{
    t_sorted *paths;

    paths = NULL;
    find_em(&paths, map, p->complete);
}
