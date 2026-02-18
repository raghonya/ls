#include "ls.h"

void	swap_ptrs(void **first, void **second)
{
	void	*tmp;

	tmp = *first;
	*first = *second;
	*second = tmp;
}

void	slice_last_chars(char **str, char c)
{
	int	len;

	if (!*str)
		return ;
	len = ft_strlen(*str);
	for (int i = len - 1; i >= 0 && *(*str + i) == c; --i)
		*(*str + i) = 0;
}

char *create_relative_path(char *path, char *name)
{
	return (ft_strjoin_w_delim(path, name, '/'));
}

t_list	*reverse_list(t_list *head)
{
	t_list	*curr;
	t_list	*prev;
	t_list	*next;
	
	prev = NULL;
	next = NULL;
	curr = head;
	while (curr != NULL)
	{
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}

	return (prev);
}
