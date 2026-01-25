#include "ls.h"

void	free_arg(void *arg)
{
	free(((arg_t *)arg)->name);
	free(((arg_t *)arg)->path);
	free(((arg_t *)arg)->perm);
	free(((arg_t *)arg)->owner);
	free(((arg_t *)arg)->group);
	free(arg);
}

int	create_arg(arg_t **data, char *path, char *name)
{
	int	ret;

	ret = 0;
	(*data) = malloc(sizeof(arg_t));
	if (!(*data))
	{
		write(2, "Malloc error\n", 13);
		return (LS_RETURN_CODE_FATAL);
	}
	(*data)->group = NULL;
	(*data)->lnk_cnt = 0;
	(*data)->owner = NULL;
	(*data)->path = ft_strdup(path);
	if (!(*data)->path)
	{
		write(2, "Malloc error\n", 13);
		free((*data));
		return (LS_RETURN_CODE_FATAL);
	}
	(*data)->name = ft_strdup(name);
	if (!(*data)->name)
	{
		write(2, "Malloc error\n", 13);
		free((*data)->path);
		free((*data));
		return (LS_RETURN_CODE_FATAL);
	}
	(*data)->perm = NULL;
	(*data)->size = 0;
	(*data)->last_modif = 0;
	(*data)->type = FILETYPE;
	(*data)->is_full_datetime = 0;

	ret = fill_arg_info((*data));
	if (ret)
		free_arg((*data));
	return (ret);
}

int	add_arg(t_list **lst, char *path)
{
	t_list	*new_node;
	arg_t	*data;
	int		ret;

	ret = create_arg(&data, path, path);
	if (!data)
		return (LS_RETURN_CODE_FATAL);
	new_node = ft_lstnew(data);
	if (!new_node)
	{
		write(2, "Malloc error\n", 13);
		free_arg(data);
		return (LS_RETURN_CODE_FATAL);
	}
	ft_lstadd_back(lst, new_node);
	return (LS_RETURN_CODE_NO_ERROR);
}

void	delete_arg(t_list **lst, t_list *node)
{
	t_list	*tmp;
	t_list	*prev;

	tmp = *lst;
	while (tmp)
	{
		if (tmp->next == node)
		{
			prev = tmp;
			prev->next = node->next;
			ft_lstdelone(node, &free_arg);
			return ;
		}
		tmp = tmp->next;
	}
}