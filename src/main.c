/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/19 21:20:17 by alkozma           #+#    #+#             */
/*   Updated: 2019/07/07 23:20:06 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lem_in.h"

int		throw_err(char *s, int stat)
{
	write(1, s, ft_strlen(s));
	return (stat);
}

int		main(void)
{
	t_map	in;

	ft_memset(&in, 0, sizeof(t_map));
	if (!read_data(&in))
		return (throw_err("ERR\n", 0));
	solve(&in);
	return (0);
}
