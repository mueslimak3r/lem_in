#include "../includes/lem_in.h"


bool    check_against(t_path *other, t_hash *hash)
{
    t_room *rooms;
    bool    collides;

    collides = false;
    rooms = other->tail;
    while (rooms)
    {
        if (rooms->hash == hash->matrix[rooms->hash])
            collides = true;
        rooms = rooms->prev;
    }
    if (collides == false)
        return (true);
    return (false);
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
    if ((*sorted)->paths)
        new->next = (*sorted)->paths;
    (*sorted)->paths = new;
}

void        check(t_sorted **sorted, t_path *path, t_hash *hash)
{
    t_path      *other_path;
    t_sorted    *new;

    new = NULL;
    push_sorted(&new, path);
    other_path = path->next;
    while (other_path != path)
    {
        if (check_against(other_path, hash))
            push_sorted(&new, other_path);
        other_path = other_path->next;
    }
    if (*sorted)
        new->next = *sorted;
    *sorted = new;
}

t_hash  *hash_path(t_path *path, t_map *map)
{
    t_hash *hash;
    t_room  *rooms;

    hash = ft_memalloc(sizeof(t_hash));
    if (!hash)
        return (NULL);
    hash->matrix = ft_memalloc((sizeof(u_int16_t) * MAX_ROOMS) + 1);
    rooms = path->tail;
    while (rooms)
    {
        if (rooms->hash != map->start && rooms->hash != map->end)
            hash->matrix[rooms->hash] = rooms->hash;
        rooms = rooms->prev;
    }
    return (hash);
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

void    find_em(t_map *map, t_path *first)
{
    t_path      *path;
    t_hash      *hash;
    t_sorted    *sorted_paths;

    sorted_paths = NULL;
    path = first;
    while (path)
    {
        hash = hash_path(path, map);
        check(&sorted_paths, path, hash);
        free(hash->matrix);
        free(hash);
        if (path->next == first)
        {
            break ;
        }
        path = path->next;
    }
    print_sorted(sorted_paths, map);
}

void    sort_complete(t_map *map, t_mypaths *p)
{
    find_em(map, p->complete);
}