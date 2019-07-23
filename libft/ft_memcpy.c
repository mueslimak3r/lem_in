/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/11 18:14:31 by alkozma           #+#    #+#             */
/*   Updated: 2019/07/19 16:40:39 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*a;
	unsigned char	*b;
	size_t			cur;
	unsigned		x;
	int				found;

	found = 0;
	a = (unsigned char*)dst;
	b = (unsigned char*)src;
	cur = 0;
	while (cur < n)
	{
		x = *(unsigned*)b;
		x & 0xFF ? a[cur] = *b : 0;
		x & 0xFF00 && cur + 1 < n ? a[cur + 1] = *(b + 1) : 0;
		x & 0xFF0000 && cur + 2 < n ? a[cur + 2] = *(b + 2) : 0;
		x & 0xFF000000 && cur + 3 < n ? a[cur + 3] = *(b + 3) : 0;
		cur += 4;
		b += 4;
	}
	return (dst);
}
