/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rec.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 17:54:26 by alkozma           #+#    #+#             */
/*   Updated: 2019/07/26 02:55:52 by calamber         ###   ########.fr       */
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
		/*
		free(tmp);
		tmp = NULL;
		*list = buf;
	}
	*/

		//CIRCULAR POP	
		if (buf->next == tmp)
			buf->next = tmp->next;
		buf->prev = tmp->prev;
		tmp->prev->next = buf;
		free(tmp);
		*list = buf;
	}
}

size_t		score_paths(t_path *in)
{
	t_path	*tmp;
	size_t		i;

	if (!in)
		return (0);
	tmp = in;
	i = 0;
	while (tmp)
	{
		i++;
		if (tmp->next == in)
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

/*
int		rec(t_sorted *augments, t_path *tmpath, t_sorted **board, t_map *in)
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
		if ((tmpscore = rec(&(paths->next), paths, board, in)) > score)
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
*/

size_t		r_solve(t_map *in, t_sorted *cluster, t_sorted **winner, t_sorted **tmp)
{
	t_sorted	*sorted_paths;
	t_path		*tmpath;
	t_path		*itt;
	size_t			hiscore;
	size_t			tmpscore;
	t_sorted	*tmp_winner;

	tmpath = cluster->paths;
	tmp_winner = NULL;
	hiscore = *tmp ? score_paths((*tmp)->paths) : 0;
	tmpscore = 0;
	sorted_paths = NULL;
	while (tmpath)
	{
		//ft_printf("%d\n", hiscore);
		new_sorted(cluster->comp, tmpath, &sorted_paths, in);
		//push_sorted(&(cluster->comp), tmpath);
		check(&sorted_paths, tmpath, sorted_paths->hash);
		itt = sorted_paths->paths;
		if (itt)
		{
			while (itt)
			{
				if ((tmpscore = r_solve(in, sorted_paths, winner, &tmp_winner)) > hiscore)
				{
					hiscore = score_paths(tmp_winner->paths);
					*tmp = tmp_winner;
				}
				if (itt->next == sorted_paths->paths)
					break ;
				itt = itt->next;
			}
		}
		else
		{
			if ((sorted_paths->flow = score_paths(sorted_paths->comp)) > hiscore)
			{
				*tmp = sorted_paths;
				hiscore = sorted_paths->flow;
			}
		}
		sorted_paths = NULL;
		if (tmpath->next == cluster->paths)
			break ;
		tmpath = tmpath->next;
	}
	return (hiscore);
}

t_sorted *solver(t_map *in, t_sorted *cluster)
{
	t_sorted *winner;
	t_sorted *tmp;
	size_t	hiscore;
	size_t	tmpscore;
	t_sorted	*cluster_tmp;

	hiscore = 0;
	tmpscore = 0;
	tmp = NULL;
	cluster_tmp = cluster;
	while (cluster_tmp)
	{
		if ((tmpscore = r_solve(in, cluster_tmp, &winner, &tmp)) > hiscore)
		{
			winner = tmp;
			hiscore = tmpscore;
		}
		if (cluster_tmp->next == cluster)
			break;
		cluster_tmp = cluster_tmp->next;
	}
	if (hiscore)
	{
		if (!winner)
		{
			ft_printf("hiscore: %zu\n", hiscore);
			ft_printf("error!\n");
			return (NULL);
		}
		t_path *fpath = winner->comp;
		ft_printf("winner with flow %zu:\n", winner->flow);
		while (fpath)
		{
			print_path(in, fpath);
			if (fpath->next == winner->comp)
				break ;
			fpath = fpath->next;
		}
	}
	return (winner);
}