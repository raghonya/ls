#include "ls.h"

void	free_arg(void *arg)
{
	free(((arg_t *)arg)->name);
	free(((arg_t *)arg)->path);
	free(((arg_t *)arg)->perm);
	free(arg);
}

int	check_arg_access(char *path, t_error *err, int *is_dir)
{
	int			ret;
	struct stat	sb;
	DIR			*dir;

	ret = lstat(path, &sb);
	if (ret != 0)
	{
		// write (2, "Error stat: ", 12);
		// write(2, path, ft_strlen(path));
		// write (2, "\n", 1);
		return (2);
	}

	*is_dir = (sb.st_mode & __S_IFMT) == __S_IFDIR;
	if (*is_dir)
	{
		dir = opendir(path);
		if (dir == NULL)
			return (2);
		closedir(dir);
	}
	return (0);
}

int	create_arg(arg_t **data, char *path, char *name)
{
	int	ret;

	ret = 0;
	(*data) = malloc(sizeof(arg_t));
	if (!(*data))
	{
		write(2, "Malloc error\n", 13);
		return (LS_ERR_RETURN_CODE_FATAL);
	}
	(*data)->group = "";
	(*data)->lnk_cnt = 0;
	(*data)->owner = "";
	(*data)->path = ft_strdup(path);
	if (!(*data)->path)
	{
		write(2, "Malloc error\n", 13);
		free((*data));
		return (LS_ERR_RETURN_CODE_FATAL);
	}
	(*data)->name = ft_strdup(name);
	if (!(*data)->name)
	{
		write(2, "Malloc error\n", 13);
		free((*data)->path);
		free((*data));
		return (LS_ERR_RETURN_CODE_FATAL);
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
		return (LS_ERR_RETURN_CODE_FATAL);
	new_node = ft_lstnew(data);
	if (!new_node)
	{
		write(2, "Malloc error\n", 13);
		free_arg(data);
		return (LS_ERR_RETURN_CODE_FATAL);
	}
	ft_lstadd_back(lst, new_node);
	return (LS_ERR_RETURN_CODE_NO_ERROR);
}

int	arg_parse(cmd_t *ls, int argc, char **argv)
{
	int		ret;
	int		is_dir;
	char	*flag_str;

	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-' && argv[i][1] != '-')
		{
			flag_str = argv[i] + 1;
			for (int flag = 0; flag_str[flag]; ++flag)
			{
				if (ls->flags == 0b11111) break ; // all flags are already set
				switch (flag_str[flag])
				{
					case 'l':
						ls->flags |= LS_OPTION_l; break ;
					case 'R':
						ls->flags |= LS_OPTION_R; break ;
					case 'a':
						ls->flags |= LS_OPTION_a; break ;
					case 'r':
						ls->flags |= LS_OPTION_r; break ;
					case 't':
						ls->flags |= LS_OPTION_t; break ;
					default:
						ls->err.code = LS_ERR_INVALID_OPTION;
						return (LS_ERR_RETURN_CODE_FATAL);
				}
			}
		}
		else
		{
			ret = check_arg_access(argv[i], &ls->err, &is_dir);
			if (ret)
			{
				printf ("Error on '%s'\n", argv[i]);
				err_type_check(ls->err);
				continue ;
			}
			if (is_dir)
				ret = add_arg(&ls->dir_args, argv[i]);
			else
				ret = add_arg(&ls->file_args, argv[i]);
			if (ret)
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