#include "ls.h"

void	free_arg(void *arg)
{
	free(((arg_t *)arg)->name);
	free(((arg_t *)arg)->path);
	free(((arg_t *)arg)->perm);
	free(arg);
}

arg_t*	create_arg(char *path, char *name)
{
	arg_t	*data;

	data = malloc(sizeof(arg_t));
	if (!data)
	{
		write(2, "Malloc error\n", 13);
		exit(2);
	}
	data->group = "";
	data->lnk_cnt = 0;
	data->owner = "";
	data->path = ft_strdup(path);
	if (!data->path)
	{
		write(2, "Malloc error\n", 13);
		exit(2);
	}
	data->name = ft_strdup(name);
	if (!data->name)
	{
		write(2, "Malloc error\n", 13);
		exit(2);
	}
	data->perm = NULL;
	data->size = 0;
	data->last_modif = 0;
	data->type = FILETYPE;

	return (data);
}

int	add_arg(cmd_t *ls, char *path)
{
	t_list	*new_node;

	new_node = ft_lstnew(create_arg(path, path));
	if (!new_node)
	{
		write(2, "Malloc error\n", 13);
		return (LS_ERR_RETURN_CODE_FATAL);
	}
	ft_lstadd_back(&ls->args, new_node);
	return (LS_ERR_RETURN_CODE_NO_ERROR);
}

int	arg_parse(cmd_t *ls, int argc, char **argv)
{
	int		ret;
	int		flag_len = 0;

	ret = 0;
	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-' && argv[i][1] != '-')
		{
			char *flag_str = argv[i] + 1;
			flag_len = ft_strlen(flag_str);
			for (int flag = 0; flag < flag_len; ++flag)
			{
				if (ls->flags == 0b11111) break ; // all flags are already set
				switch (flag_str[flag])
				{
					case 'l':
						ls->flags |= LS_OPTION_l;
						break ;
					case 'R':
						ls->flags |= LS_OPTION_R;
						break ;
					case 'a':
						ls->flags |= LS_OPTION_a;
						break ;
					case 'r':
						ls->flags |= LS_OPTION_r;
						break ;
					case 't':
						ls->flags |= LS_OPTION_t;
						break ;
					default:
						ls->err.code = LS_ERR_INVALID_OPTION;
						return (LS_ERR_RETURN_CODE_FATAL);
				}
			}
		}
		else
		{
			if (add_arg(ls, argv[i]))
				return (LS_ERR_RETURN_CODE_FATAL);
		}
	}
	return (0);
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