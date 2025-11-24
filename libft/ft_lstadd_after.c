#include "libft.h"

void	ft_lstadd_after(t_list **lst, t_list *new)
{
	if (!*lst && new)
	{
		*lst = new;
		return ;
	}
	else if (!new)
		return ;
	if (lst && new)
	{
		new->next = (*lst)->next;
		(*lst)->next = new;
	}
}
