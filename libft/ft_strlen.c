/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 09:43:50 by alkozma           #+#    #+#             */
/*   Updated: 2019/07/22 17:48:45 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Returns the length of a string as an integer.
**
** This strlen uses pseudo-vectorization in order to
** count in chunks of 4 instead of checking char by
** char. The string is cast to an unsigned array,
** allowing us to read past the end of the string's
** memory to detect the null byte in chunks without
** segfaulting or read errors.
*/

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t		len;
	unsigned	x;
	int			i;

	len = 0;
	i = 0;
	while (s)
	{
		x = *(unsigned*)s;
		if ((x & 0xFF) == 0)
			return (len);
		else if ((x & 0xFF00) == 0)
			return (len + 1);
		else if ((x & 0xFF0000) == 0)
			return (len + 2);
		else if ((x & 0xFF000000) == 0)
			return (len + 3);
		len += 4;
		s += 4;
	}
	return (len);
}
