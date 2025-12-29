#include "ls.h"

/*
◦ write
◦ opendir
◦ readdir
◦ closedir
◦ stat
◦ lstat
◦ getpwuid
◦ getgrgid
◦ listxattr
◦ getxattr
◦ time
◦ ctime
◦ readlink
◦ malloc
◦ free
◦ perror
◦ strerror
◦ exit
*/

int		show_contents(t_list *node, t_info_max_lengths *max_lengths, uint32_t opts, int print_dir_name);
int		count_arg_info_lengths(t_list *order, t_info_max_lengths *max_lengths);

int		read_link(char *link, struct stat *statbuf)
{

	char	*buf;
	ssize_t	bufsiz, nbytes;

	bufsiz = statbuf->st_size + 1;

	if (statbuf->st_size == 0)
		bufsiz = PATH_MAX;

	buf = malloc(bufsiz);
	if (buf == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	nbytes = readlink(link, buf, bufsiz);
	if (nbytes == -1) {
		perror("readlink");
		exit(EXIT_FAILURE);
	}

	printf("'%s' points to '%.*s'\n", link, (int) nbytes, buf);

	free(buf);
}

int		check_recursion(t_list *order, t_list **subdirs)
{
	int		ret;
	arg_t	*data;
	arg_t	*tmp_arg;
	t_list	*tmp_lst;

	ret = LS_ERR_RETURN_CODE_NO_ERROR;
	while (order)
	{
		data = order->data;
		if (data->type == __S_IFDIR \
		&& ft_strcmp(data->name, ".") != 0 \
		&& ft_strcmp(data->name, "..") != 0)
		{
			tmp_arg = NULL;
			ret = create_arg(&tmp_arg, data->path, data->name);
			if (ret)
				return (LS_ERR_RETURN_CODE_FATAL);
			t_list *tmp_lst = NULL;
			tmp_lst = ft_lstnew(tmp_arg);
			if (!tmp_lst)
			{
				// write(2, "Malloc error\n", 13);
				free_arg(tmp_arg);
				return (LS_ERR_RETURN_CODE_FATAL);
			}
			ft_lstadd_back(subdirs, tmp_lst);
		}
		order = order->next;
	}
	return (ret);
}

int		check_dir_contents(t_list **order, t_list **subdirs, char *path, uint32_t opts, int triggers)
{
	int				ret;
	DIR				*dir;
	struct dirent	*elem;
	char			*new_path;

	ret = 0;
	// order = NULL;
	new_path = NULL;
	dir = opendir(path);
	if (!dir){
		printf("opendir on %s failed\n", path);
		return (LS_ERR_RETURN_CODE_FATAL);
	}
	if (triggers & PRINT_DIR_NAME)
	{
		write(1, path, ft_strlen(path));
		write(1, ":\n", 2);
	}
	slice_last_chars(&path, '/');
	elem = readdir(dir);
	while (elem != NULL)
	// for (elem = readdir(dir); elem != NULL; elem = readdir(dir))
	{
		if (!(LS_OPTION_a & opts) && elem->d_name[0] == '.')
		{
			elem = readdir(dir);
			continue ;
		}
		new_path = create_relative_path(path, elem->d_name);
		if (!new_path)
		{
			closedir(dir);
			ft_lstclear(order, &free_arg);
			return (LS_ERR_RETURN_CODE_FATAL);
		}
		arg_t *tmp_arg = NULL;
		ret = create_arg(&tmp_arg, new_path, elem->d_name);
		if (ret)
		{
			free(new_path);
			closedir(dir);
			ft_lstclear(order, &free_arg);
			return (LS_ERR_RETURN_CODE_FATAL);
		}
		t_list *tmp_lst = ft_lstnew(tmp_arg);
		if (!tmp_lst)
		{
			write(2, "Malloc error\n", 13);
			free_arg(tmp_arg);
			free(new_path);
			closedir(dir);
			ft_lstclear(order, &free_arg);
			return (LS_ERR_RETURN_CODE_FATAL);
		}
		ft_lstadd_back(order, tmp_lst);
		if (ret)
		{
			free(new_path);
			closedir(dir);
			ft_lstclear(order, &free_arg);
			return (ret);
		}
		free(new_path);
		elem = readdir(dir);
	}
	sort_with_opts(order, opts);
	if (opts & LS_OPTION_R)
	{
		ret = check_recursion(*order, subdirs);
		if (ret)
		{
			closedir(dir);
			ft_lstclear(order, &free_arg);
			return (ret);
		}
	}
	closedir(dir);
	// ft_lstclear(order, &free_arg);

	return (0);
}

int		check_last_dir_recursive(t_list *subdirs, char *path, char *last_dir, int *triggers)
{
	char *tmp_str;
	char *tmp_str_ptr;
	
	tmp_str = ft_strdup(path);
	if (!tmp_str)
		return (LS_ERR_RETURN_CODE_FATAL);
	tmp_str_ptr = tmp_str;
	if (*triggers & LAST_ARG_RECURSION)
	{
		if (subdirs)
		{
			if (path && ft_strcmp(path, last_dir) == 0)
			{
				*triggers &= ~LAST_ARG;
				free(tmp_str);
				tmp_str = create_relative_path(last_dir, ((arg_t *)ft_lstlast(subdirs)->data)->name);
				if (!tmp_str)
					return (LS_ERR_RETURN_CODE_FATAL);
			}
		}
		else
		{
			// printf ("argpath inside: %s\n", path);
			if (ft_strcmp(path, last_dir) == 0)
				*triggers |= LAST_ARG;
		}
	}

	return (LS_ERR_RETURN_CODE_NO_ERROR);
}

int		open_dir(arg_t *arg, char *last_dir, t_info_max_lengths *max_lengths, uint32_t opts, int triggers)
{
	int		ret;
	char	*tmp_str;
	t_list	*tmp;
	t_list	*order;
	t_list	*subdirs;;

	order = NULL;
	subdirs = NULL;
	tmp_str = ft_strdup(arg->path);
	if (!tmp_str)
		return (LS_ERR_RETURN_CODE_FATAL);
	ret = check_dir_contents(&order, &subdirs, arg->path, opts, triggers);
	if (ret)
	{
		free(tmp_str);
		return (ret);
	}
	if (triggers & LAST_ARG_RECURSION)
	{
		if (subdirs)
		{
			if (ft_strcmp(arg->path, last_dir) == 0)
			{
				triggers &= ~LAST_ARG;
				free(tmp_str);
				tmp_str = create_relative_path(last_dir, ((arg_t *)ft_lstlast(subdirs)->data)->name);
				if (!tmp_str)
					return (LS_ERR_RETURN_CODE_FATAL);
			}
		}
		else
		{
			if (ft_strcmp(arg->path, last_dir) == 0)
				triggers |= LAST_ARG;
		}
	}
	print_ordered(order, max_lengths, opts, triggers);
	tmp = subdirs;
	while (tmp)
	{
		ret = open_dir(tmp->data, tmp_str, max_lengths, opts, triggers);
		// printf("freeing '%s'\n", tmp_str);
		if (ret)
		{
			write (1, "error\n", 6);
			tmp = tmp->next;
			continue ;
		}
		tmp = tmp->next;
	}
	free(tmp_str);
	ft_lstclear(&order, &free_arg);
	ft_lstclear(&subdirs, &free_arg);
	// if (ft_strcmp(rec_entry_path, path))
	// 	write (1, "\n", 1);
	return (ret);
}

int		show_contents(t_list *node, t_info_max_lengths *max_lengths, uint32_t opts, int triggers)
{
	if (!node->next)
	{
		triggers |= LAST_ARG;
		if (opts & LS_OPTION_R)
			triggers |= LAST_ARG_RECURSION;
	}
	return (open_dir(node->data, ((arg_t *)node->data)->path, max_lengths, opts, triggers));
}

int		initialize_cmd(cmd_t **ls)
{
	*ls = malloc (sizeof(cmd_t));
	if (!*ls) 
		return (LS_ERR_RETURN_CODE_FATAL);
	(*ls)->opts = 0;
	(*ls)->dir_args = NULL;
	(*ls)->file_args = NULL;
	// (*ls)->parent_path = NULL;
	(*ls)->triggers = 0;
	(*ls)->err.code = LS_ERR_RETURN_CODE_NO_ERROR;
	(*ls)->err.message = NULL;
	(*ls)->err.type = 0;
	ft_memset(&(*ls)->max_lengths, 0, sizeof(t_info_max_lengths));

	return (LS_ERR_RETURN_CODE_NO_ERROR);
}

void print_arg_lens(t_info_max_lengths *max_lengths)
{
	printf ("Link cnt len: %zu\n", max_lengths->lnk_cnt_len);
	printf ("Owner len: %zu\n", max_lengths->owner_len);
	printf ("Group len: %zu\n", max_lengths->group_len);
	printf ("Major len: %zu\n", max_lengths->major_len);
	printf ("Minor len: %zu\n", max_lengths->minor_len);
	printf ("Size len: %zu\n", max_lengths->size_len);
	printf ("Day len: %zu\n", max_lengths->day_len);
}

int		main(int argc, char **argv)
{
	int		ret;
	cmd_t	*ls;
	t_list	*tmp_lst;
	
	if (argc < 1)
		return (1);
	initialize_cmd(&ls);
	ret = arg_parse(ls, argc, argv);
	// printf ("err : %d\n" ,ret);
	if (ret)
	{
		// if (err_type_check(ls->err) & EXIT_FAILURE)
		// {
		// 	// free all info inside ls
		// 	// free(ls);
		// 	return (EXIT_FAILURE);
		// }
		exit (2);
	}
	if (ls->file_args == NULL && ls->dir_args == NULL)
		add_arg(&ls->dir_args, ".");
	if ((ls->dir_args && ls->dir_args->next != NULL) || \
	(ls->file_args && ls->dir_args != NULL) ||
	ls->opts & LS_OPTION_R)
		ls->triggers |= PRINT_DIR_NAME;
	sort_with_opts(&ls->dir_args, ls->opts);
	sort_with_opts(&ls->file_args, ls->opts);
	ret = count_arg_info_lengths(ls->dir_args, &ls->max_lengths);
	if (ret)
	{
		// write (1, "error\n", 6);
		// clean up ls
		return (LS_ERR_RETURN_CODE_FATAL);
	}
	ret = count_arg_info_lengths(ls->file_args, &ls->max_lengths);
	if (ret)
	{
		// write (1, "error\n", 6);
		// clean up ls
		return (LS_ERR_RETURN_CODE_FATAL);
	}
	if (ls->file_args)
		print_ordered(ls->file_args, &ls->max_lengths, ls->opts, ls->triggers | DONT_CHECK_OPT_a | DONT_PRINT_TOTAL);
	for (tmp_lst = ls->dir_args; tmp_lst != NULL; tmp_lst = tmp_lst->next)
	{
		ls->parent_path = ((arg_t *)tmp_lst->data)->path;
		ret = show_contents(tmp_lst, &ls->max_lengths, ls->opts, ls->triggers);
		if (ret)
		{
			// write (1, "error\n", 6);
			// write (1, ((arg_t *)tmp_lst->data)->path, ft_strlen(((arg_t *)tmp_lst->data)->path));
			continue ;
		}
	}
	ft_lstclear(&ls->dir_args, &free_arg);
	ft_lstclear(&ls->file_args, &free_arg);
	free(ls);
	return (0);
}
