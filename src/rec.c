/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rec.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 17:54:26 by alkozma           #+#    #+#             */
/*   Updated: 2019/07/23 19:09:33 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lem_in.h"

/*int		list_compare(t_solved **a, t_solved **b)
{
	t_path *b_paths;
	bool	conflicts;

	conflicts = false;
	b_paths = (*b)->paths;
	while(b_paths)
	{
		conflicts = !check_against(b_paths, (*a)->hash);
		if (b_paths->next == (*b)->paths)
			break ;
		b_paths = b_paths->next;
	}
	return (conflicts);
}*/

void	pop_head(t_sorted **list)
{
	t_sorted	*tmp;
	t_sorted	*buf;

	tmp = *list;
	buf = (*list)->next;
	tmp->prev->next = tmp->next;
	free(tmp);
	*list = buf;
}

int		score_paths(t_sorted **in)
{
	t_path	*tmp;
	int		i;

	if (!in || !*in)
		return (0);
	tmp = (*in)->paths;
	i = 0;
	while (tmp)
	{
		i++;
		if (tmp->next == (*in)->paths)
			break ;
		tmp = tmp->next;
	}
	return (i);
}

int		rec(t_sorted **augments, t_sorted **board, t_map *in)
{
	t_sorted	*a;
	t_sorted	*tmp;
	t_path		*paths;
	t_hash		*hash;
	int			score;
	int			tmpscore;

	score = score_paths(board);
	a = *augments;
	paths = a->paths;
	tmp = NULL;
	hash = NULL;
	while (paths)
	{
		push_sorted(board, paths);
		if ((tmpscore = rec(augments, board, in)) > score)
			score = tmpscore;
		else
			pop_head(board);
		if (paths->next == a->paths)
		{
			ft_printf("%d\n", 1);
			break ;
		}
		paths = paths->next;
	}
	return (score);
}

int		solver(t_map *in, t_path **paths)
{
	t_sorted	*sorted_paths;
	t_sorted	*tmp;
	t_path		*tmpath;
	t_sorted		*itt;
	t_hash		*hash;
	int			hiscore;
	int			tmpscore;
	t_sorted	*winner;

	hiscore = 0;
	tmpscore = 0;
	sorted_paths = NULL;
	tmp = NULL;
	tmpath = *paths;
	hash = NULL;
	while (tmpath)
	{
		ft_printf("%d\n", hiscore);
		hash_path(&hash, tmpath, in);
		check(&sorted_paths, tmpath, hash);
		itt = sorted_paths;
		if ((tmpscore = rec(&itt, &tmp, in)) > hiscore)
		{
			winner = (tmp);
			hiscore = tmpscore;
		}
		if (tmpath->next == (*paths))
			break ;
		tmpath = tmpath->next;
	}
	return (hiscore);
}
