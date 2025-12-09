/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raghonya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:22:57 by raghonya          #+#    #+#             */
/*   Updated: 2023/01/16 16:23:03 by raghonya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	i;
	size_t	size;
	char	*dup;

	i = 0;
	size = ft_strlen(s);
	dup = malloc(sizeof(char) * (size + 1));
	if (!dup)
		return (NULL);
	while (i < size)
	{
		*(dup + i) = *(s + i);
		i++;
	}
	*(dup + i) = 0;
	return (dup);
}
