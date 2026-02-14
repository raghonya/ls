#include "ls.h"

void	print(t_list *lst)
{
	while (lst)
	{
		arg_t *data = lst->data;
		printf("%s\n", data->name);
		lst = lst->next;
	}
}

void	print_all_info(arg_t *elem)
{
	printf ("perms: %s\n", elem->perm);
	printf ("links: %ld\n", elem->lnk_cnt);
	printf ("owner: %s\n", elem->owner);
	printf ("group: %s\n", elem->group);
	printf ("size: %ld\n", elem->size);
	printf ("time: %ld\n", (uintmax_t)elem->last_modif);
	printf ("path: %s\n", elem->path);
	printf ("name: %s\n", elem->name);
	printf ("blocks: %ld\n", elem->blocks);
}

void	err_exit(int condition, char *message, int code)
{
	if (condition)
	{
		write (2, message, ft_strlen(message));
		write (2, "\n", 1);
		exit (code);
	}
}

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
