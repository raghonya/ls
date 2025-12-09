#include "ls.h"

static char	*remove_symbols(char *str)
{
	char	*copy;

	copy = ft_strdup(str);
	if (!copy)
		return (copy);
	for (int i = 0; copy[i]; ++i)
	{
		if (!ft_isalnum(copy[i]))
		{
			for (int j = i; copy[j]; ++j)
				copy[j] = copy[j + 1];
			i--;
		}
	}
	return (copy);
}

static int	compare_names(t_list *first, t_list *second)
{
	char	*tmpc1;
	char	*tmpc2;
	char	*tmpc1_removed;
	char	*tmpc2_removed;

	tmpc1 = str_to_lower(((arg_t *)first->data)->name);
	tmpc2 = str_to_lower(((arg_t *)second->data)->name);
	tmpc1_removed = remove_symbols(tmpc1);
	tmpc2_removed = remove_symbols(tmpc2);
	if (ft_strcmp(tmpc1_removed, tmpc2_removed) == 0)
	{
		if(ft_strcmp(tmpc1, tmpc2) > 0)
			swap_ptrs(&first->data, &second->data);
	}
	else
	{
		if(ft_strcmp(tmpc1_removed, tmpc2_removed) > 0)
			swap_ptrs(&first->data, &second->data);
	}
	free(tmpc1_removed);
	free(tmpc2_removed);
	free(tmpc1);
	free(tmpc2);
}

void sort_list(t_list **lst)
{
	t_list	*node;
	t_list	*temp;
	t_list	*temp2;

	temp = NULL;
	node = *lst;
	while (node != NULL)
	{
		temp = node;
		temp2 = temp->next;
		while (temp2 != NULL)
		{
			compare_names(temp, temp2);
			temp2 = temp2->next;
		}
		node = node->next;
	}
}
