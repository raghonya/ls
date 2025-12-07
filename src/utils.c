#include "ls.h"

void	print_all_info(arg_t *elem)
{
	printf ("perms: %s\n", elem->perm);
	printf ("links: %d\n", elem->lnk_cnt);
	printf ("owner: %s\n", elem->owner);
	printf ("group: %s\n", elem->group);
	printf ("size: %d\n", elem->size);
	printf ("time: %d\n", (uintmax_t)elem->last_modif);
	printf ("path: %s\n", elem->path);
	printf ("name: %s\n", elem->name);
	printf ("blocks: %d\n", elem->blocks);
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

char *str_to_lower(char *str)
{
	size_t	len;
	char	*low_str;

	len = ft_strlen(str);
	low_str = malloc(sizeof(char) * (len + 1));
	if (!low_str)
		return (NULL);
	for (int i = 0; i < len; ++i)
		low_str[i] = ft_tolower(str[i]);
	return (low_str);
}

// void	swap_args(void **first, void **second)
// {
// 	void *tmp;

// 	tmp = *first;
// 	*first = *second;
// 	*second = tmp;
// }

void	slice_last_chars(char **str, char c)
{
	int	len;

	len = ft_strlen(*str);
	for (int i = len - 1; i >= 0 && *(*str + i) == c; --i)
		*(*str + i) = 0;
}

char *create_relative_path(char *path, char *name)
{
	char	*rel_path;
	char	*tmp;

	rel_path = ft_strjoin(path, "/");
	if (!rel_path)
		return (NULL);
	tmp = rel_path;
	rel_path = ft_strjoin(rel_path, name);
	free(tmp);
	if (!rel_path)
		return (NULL);

	return (rel_path);
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
