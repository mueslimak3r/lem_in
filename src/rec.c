/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rec.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 17:54:26 by alkozma           #+#    #+#             */
/*   Updated: 2019/07/24 18:58:30 by calamber         ###   ########.fr       */
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
	buf = tmp->next;
	if (tmp == buf)
	{
		free(*list);
		*list = NULL;
	}
	else
	{
		free(tmp);
		tmp = NULL;
		*list = tmp;
	}

	// CIRCULAR POP	
	/*tmp = *list;
	buf = (*list)->next;
	tmp->prev->next = tmp->next;
	free(tmp);
	*list = buf;*/
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

void	print_list(t_map *in, t_sorted **s)
{
	t_sorted	*tmp;

	tmp = *s;
	while (tmp)
	{
		print_path(in, tmp->paths);
		if (tmp->next == *s)
			break ;
		tmp = tmp->next;
	}
}

int		rec(t_sorted **augments, t_path *tmpath, t_sorted **board, t_map *in)
{
	t_sorted	*a;
	t_sorted	*tmp;
	t_path		*paths;
	t_hash		*hash;
	int			score;
	int			tmpscore;

	score = score_paths(board);
	a = *augments;
	if (!a)
		return (0);
	paths = a->paths;
	tmp = NULL;
	hash = NULL;
	//print_path(in, a->paths);
	while (paths)	// currently doesnt go through the augment list properly
	{
		push_sorted(board, paths);
		//print_list(in, board);
		if ((tmpscore = rec(&(a->next), board, in)) > score)
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

int		solver(t_map *in, t_sorted *cluster)
{
	t_sorted	*sorted_paths;
	t_sorted	*tmp;
	t_path		*tmpath;
	t_sorted		*itt;
	t_hash		*hash;
	int			hiscore;
	int			tmpscore;
	t_sorted	*winner;
	t_paths		*paths;

	paths = cluster->paths;
	winner = NULL;
	hiscore = 0;
	tmpscore = 0;
	sorted_paths = NULL;
	tmp = NULL;
	tmpath = *paths;
	hash = NULL;
	while (tmpath)
	{
		//ft_printf("%d\n", hiscore);
		new_sorted(NULL, tmpath, &sorted_paths, in);
		check(&sorted_paths, tmpath, hash);
		itt = sorted_paths;
		while (itt)
		{
			if ((tmpscore = rec(&itt, tmpath, &tmp, in)) > hiscore)
			{
				winner = (tmp);
				hiscore = tmpscore;
			}
			if (itt->next == sorted_paths)
				break ;
			itt = itt->next;
		}
		sorted_paths = NULL;
        hash = NULL;
		if (tmpath->next == (*paths))
			break ;
		tmpath = tmpath->next;
	}
	if (hiscore)
	{
		t_path *fpath = winner;
		ft_printf("winner:\n");
		while (fpath)
		{
			print_path(in, fpath);
			if (fpath->next == winner)
				break ;
			fpath = fpath->next;
		}
	}
	return (hiscore);
}
