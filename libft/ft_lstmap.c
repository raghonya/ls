/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raghonya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 14:34:58 by raghonya          #+#    #+#             */
/*   Updated: 2023/01/16 14:35:12 by raghonya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*a;

	a = NULL;
	while (lst)
	{
		new = ft_lstnew (f(lst->data));
		if (!new)
		{
			ft_lstclear (&new, del);
			return (NULL);
		}
		ft_lstadd_back(&a, new);
		lst = lst->next;
	}
	return (a);
}
