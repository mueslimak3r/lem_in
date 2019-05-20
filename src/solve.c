#include "../includes/lem_in.h"

// pull_path

void        print_paths(t_map *map, t_path *paths, char *name)
{
    t_room  *rooms;

    ft_printf("%s:\n", name);
    while (paths)
    {
        rooms = paths->tail;
        while (rooms)
        {
            ft_printf("%s->", map->hash_info.data[rooms->hash]);
            rooms = rooms->prev;
        }
        ft_printf("  %p\n", paths);
        if (paths->is_last || paths->next == paths)
            break ;
        paths = paths->next;
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
    temp->next = *b;
    temp->prev = NULL;
    temp->is_last = (*b) ? false : true;
    *b = temp;
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

void        mod_path(t_map *map, t_mypaths *p)
{
    int     i;

    i = 0;
    if ((*p).paths->tail->hash == map->end)
    {
        //ft_printf("path complete!\n");
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
    }
}

int         solve(t_map *map)
{
    t_mypaths   p;

    if (!map->start || !map->end)
        return (0);
    ft_memset(&p, 0, sizeof(t_mypaths));
    p.complete = NULL;
    p.paths = NULL;
    p.removed = NULL;
    push_path(&p.paths, NULL, map->start);
    map->size = 1;
    /*
    while (p.paths)
    {
        print_paths(map, p.paths, "paths");
        //print_paths(map, p.complete, "complete");
        ft_printf("done printing\n");
        mod_path(map, &p);
        ft_printf("parsed path\n");
    }
    */
    while (p.paths)
   {
   //print_paths(map, p.paths, "paths");
   mod_path(map, &p);
    if (map->size % 100000 == 0)
        printf("%llu paths\n", map->size);
   //print_paths(map, p.paths, "paths");
    }
    count_nodes(p.paths);
    count_nodes(p.complete);
    print_paths(map, p.complete, "complete");
    return (1);
}