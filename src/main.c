/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/19 21:20:17 by alkozma           #+#    #+#             */
/*   Updated: 2019/05/19 22:38:33 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lem_in.h"

int		main(void)
{
	t_map	*in;

	in = (t_map*)malloc(sizeof(t_map));
	if (!read_data(in))
		return (ft_printf("ERR\n"));
	ft_printf("%s\n", in->hash_info.data[in->end]);
}
