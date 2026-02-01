#include "ls.h"

void	sort_with_opts(t_list **lst, uint32_t flags)
{
	sort_list(lst, (flags & LS_OPTION_t) ? SORT_BY_TIME : SORT_BY_NAME);
	if (flags & LS_OPTION_r)
		*lst = reverse_list(*lst);
	// printf("Sorted list with flags (flags = %u):\n", flags);
	// print(*lst);
	// printf("Sorted list end\n", flags);
}

static char	*remove_symbols(char *str)
{
	char	*copy;

	copy = ft_strdup(str);
	if (!copy)
	{
		ft_strcpy(g_err.name, str);
		g_err.type = LS_ERR_NOT_ENOUGH_MEMORY;
		return (copy);
	}
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

	tmpc1 = ft_new_str_tolower(((arg_t *)first->data)->name);
	tmpc2 = ft_new_str_tolower(((arg_t *)second->data)->name);
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

void	compare_times(t_list *first, t_list *second)
{
	time_t	first_time;
	time_t	second_time;

	first_time = ((arg_t *)first->data)->last_modif;
	second_time = ((arg_t *)second->data)->last_modif;
	if (first_time < second_time)
		swap_ptrs(&first->data, &second->data);
}

void	sort_list(t_list **lst, int sort_by)
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
			if (sort_by == SORT_BY_TIME)
				compare_times(temp, temp2);
			else if (sort_by == SORT_BY_NAME)
				compare_names(temp, temp2);
			temp2 = temp2->next;
		}
		node = node->next;
	}	
}
