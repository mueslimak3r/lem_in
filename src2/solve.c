/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 03:05:08 by alkozma           #+#    #+#             */
/*   Updated: 2019/07/30 00:15:21 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lem_in.h"

// DEAR MORNING ALEX
// SEGV ON MAPS

t_room	*find_room(t_map *in, uint16_t hash)
{
	t_room	*tmp;

	tmp = in->hash_info.list[hash % 5000];
	while (tmp)
	{
		if (tmp->hash == hash)
			return (tmp);
		if (tmp->prev == in->hash_info.list[hash % 5000])
			return (NULL);
		tmp = tmp->prev;
	}
	return (tmp);
}

void	inform_room(t_map *in, uint16_t hash, t_path *path, int direction)
{
	t_room			*rm;
	t_cluster_node	*new;
	t_cluster_node	*tmp;

	rm = find_room(in, hash);
	if (!rm)
		return ;
	new = malloc(sizeof(t_cluster_node));
	new->path = path;
	new->next = NULL;
	tmp = direction ? rm->from_end : rm->from_start;
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp ? (tmp->next = new) : (tmp = new);
}

void	print_links(t_map *in, uint16_t hash)
{
	t_room	*tmp;

	tmp = in->hash_info.list[hash % 5000]->links;
	/*while (tmp)
	{
		ft_printf("%lld\n", tmp->hash);
		tmp = tmp->prev;
	}*/
}

int	check_overlaps(t_room *tail, uint16_t hash)
{
	t_room	*tmp;

	tmp = tail;
	while (tmp)
	{
		if (tmp->hash == hash)
			return (1);
		if (tmp->prev == tail || tmp->prev == tmp)
			return (0);
		tmp = tmp->prev;
	}
	return (0);
}

void	add_room(t_path *new_path, t_room *old, uint16_t new_hash, int direction, t_map *in)
{
	t_room	*new;
	t_room	*tmp;

	new = malloc(sizeof(t_room));
	new->hash = new_hash;
	new->prev = old ? old : new;
	tmp = find_room(in, new_hash);
	new->from_start = tmp ? tmp->from_start : NULL;
	new->from_end = tmp ? tmp->from_end : NULL;
	new_path->tail = new;
	new->id = tmp ? tmp->id : NULL;
	new->links = tmp ? in->hash_info.list[new_hash % 5000]->links : NULL;
	inform_room(in, new_hash, new_path, direction);
}

void	push_path(t_cluster_node **head, t_room *old, uint16_t new_hash, int direction, t_map *in)
{
	t_path			*new;
	t_cluster_node	*tmp;
	t_cluster_node	*out;

	new = malloc(sizeof(t_path));
	if (new_hash > 0)
		add_room(new, old, new_hash, direction, in);
	else
		new->tail = old;
	new->is_last = true;
	tmp = *head;
	while (tmp && tmp->next)
	{
		if (tmp->next == *head)
			break ;
		tmp = tmp->next;
	}
	out = malloc(sizeof(t_cluster_node));
	out->path = new;
	out->next = NULL;
	tmp ? (tmp->next = out) : (tmp = out);
	*head = tmp;
}

void	mod_path_2(t_map *in, t_cluster_node **n, int mode)
{
	t_cluster_node	*orig;
	t_room			*links;

	orig = *n;
	if (!orig)
		return ;
	if ((orig->path->tail->hash == in->end && !mode) || (orig->path->tail->hash == in->start && mode))
	{
		push_path(&g_completed_paths, orig->path->tail, 0, mode, in);
		return ;
	}
	links = orig->path->tail->links;
	while (links)
	{
		if (!check_overlaps(orig->path->tail, links->hash))
			push_path(&orig, orig->path->tail, links->hash, mode, in);
		if (links == orig->path->tail->links)
			break ;
		links = links->prev;
	}
}

int		get_depth(t_path **path)
{
	t_room	*tmp;
	int		ret;
	
	tmp = (*path)->tail;
	ret = 0;
	while (tmp)
	{
		ret++;
		if (tmp->prev == (*path)->tail || tmp->prev == tmp)
			break ;
		tmp = tmp->prev;
	}
	return (ret);
}

int		solve(t_map *in)
{
	t_cluster_node	*head;
	t_cluster_node	*tail;
	int				i;

	i = 0;
	if (!in->start || !in->end)
		return (0);
	head = NULL;
	tail = NULL;
	push_path(&head, NULL, in->start, 0, in);
	push_path(&tail, NULL, in->end, 1, in);
	while (head || tail)
	{
		ft_printf("%d\n", i);
		mod_path_2(in, &head, 0);
		mod_path_2(in, &tail, 1);
		head = head->next;
		tail = tail->next;
		i++;
	}
	return (0);
}
