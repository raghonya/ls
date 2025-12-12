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

void	print(t_list *lst)
{
	while (lst)
	{
		arg_t *data = lst->data;
		printf("%s\n", data->name);
		lst = lst->next;
	}
}

int	show_contents(t_list *node, uint32_t flags);

void err_type_check(t_error err)
{
	// char	*err_message = "";

	switch (0)
	{
		case LS_ERR_INVALID_OPTION:
			ft_putendl_fd(err.message, 1);
			exit(2);
			// err_message = LS_ERR_MESSAGE_INVALID_OPTION; //pti poxvi code y flagi het
			// printf ("ooooo\n");
			// write (1, "ls: invalid")
			break;
		case LS_ERR_NO_SUCH_FILE_OR_DIRECTORY:
			write (1, "", 1);
		case LS_ERR_PERMISSION_DENIED:
			write (1, "", 1);
	}

	// err_message = "yo";
	// ft_putendl_fd(err_message, STDERR_FILENO);
	// exit(exit_code); // need to be changed
}

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

int	print_name(arg_t *arg, int is_last, uint32_t flags)
{
	int			ret;
	char		*tmp;
	struct stat	statbuf;
	char		permissions[11] = "-rwxrwxrwx";

	ret = 0;
	if (!(LS_OPTION_a & flags) && arg->name[0] == '.')
		return (0);
	if (LS_OPTION_l & flags)
	{
		// type and permissions
		write (1, arg->perm, ft_strlen(arg->perm));
		write (1, " ", 1);
		
		// link count
		tmp = ft_itoa(arg->lnk_cnt);
		if (!tmp)
		{

		}
		write (1, tmp, ft_strlen(tmp));
		write (1, " ", 1);
		free(tmp);
		
		// owner
		write (1, arg->owner, ft_strlen(arg->owner));
		write (1, " ", 1);
		
		// group
		write (1, arg->group, ft_strlen(arg->group));
		write (1, " ", 1);
		
		// size
		tmp = ft_itoa(arg->size);
		if (!tmp)
		{

		}
		write (1, tmp, ft_strlen(tmp));
		write (1, " ", 1);
		free(tmp);
		
		// last modified time
		tmp = ctime(&arg->last_modif);
		write(1, tmp, ft_strlen(tmp) - 1);
		write(1, " ", 1);

		// name
		write (1, arg->name, ft_strlen(arg->name));
		write (1, "\n", 1);
	}
	else
	{
		write(1, arg->name, ft_strlen(arg->name));
		if (!is_last)
			write (1, " ", 1);
		else
			write (1, "\n", 1);
			
	}
	return (ret);
}

int	print_ordered(t_list *order, uint32_t flags)
{
	while (order)
	{
		print_name(order->data, order->next == NULL, flags);
		order = order->next;
	}
}

void	check_recursion(t_list *order, t_list **subdirs)
{
	arg_t	*data;

	while (order)
	{
		data = order->data;
		if (data->type == DT_DIR \
		&& ft_strcmp(data->name, ".") != 0 \
		&& ft_strcmp(data->name, "..") != 0)
			ft_lstadd_back(subdirs, ft_lstnew(create_arg(data->path, data->name)));
		order = order->next;
	}
}

int	check_dir_contents(t_list **subdirs, char *path, uint32_t flags)
{
	DIR				*dir;
	struct dirent	*elem;
	t_list			*order;
	char			*new_path;

	order = NULL;
	new_path = NULL;
	dir = opendir(path);
	if (!dir){
		puts("opendir");
		return (2);
	}
	elem = readdir(dir);
	if (flags & LS_OPTION_R)
	{
		write(1, path, ft_strlen(path));
		write(1, ":\n", 2);
	}
	slice_last_chars(&path, '/');
	while (elem != NULL)
	{
		if (!(LS_OPTION_a & flags) && elem->d_name[0] == '.')
		{
			elem = readdir(dir);
			continue ;
		}
		new_path = create_relative_path(path, elem->d_name);
		err_exit(!new_path, "Malloc error", 2);
		t_list *tmp_lst;
		tmp_lst = ft_lstnew(create_arg(new_path, elem->d_name));
		err_exit(!tmp_lst, "Malloc error", 2);
		((arg_t *)tmp_lst->data)->type = elem->d_type;
		int ret = fill_arg_info(tmp_lst->data);
		ft_lstadd_back(&order, tmp_lst);
		free(new_path);
		elem = readdir(dir);
	}
	sort_list(&order, (flags & LS_OPTION_t) ? SORT_BY_TIME : SORT_BY_NAME);
	if (flags & LS_OPTION_r)
		order = reverse_list(order);
	if (flags & LS_OPTION_R)
		check_recursion(order, subdirs);
	closedir(dir);
	print_ordered(order, flags);
	ft_lstclear(&order, &free_arg);

	return (0);
}

int	open_dir(char *path, uint32_t flags)
{
	int		ret;
	t_list	*tmp;
	t_list	*subdirs;

	subdirs = NULL;
	ret = check_dir_contents(&subdirs, path, flags);
	if (ret)
		return (ret);
	tmp = subdirs;
	while (tmp != NULL)
	{
		open_dir(((arg_t *)tmp->data)->path, flags);
		tmp = tmp->next;
	}
	ft_lstclear(&subdirs, &free_arg);

	return (ret);
}

int	format_output(t_list *parent_node, uint32_t flags)
{
	int			ret;
	arg_t		*data = ((arg_t *)parent_node->data);

	ret = 0;
	if (data->type == __S_IFDIR)
		ret = open_dir(data->path, flags);
	else
		ret = print_name(data, 0, flags);
	return (ret);
}

int	show_contents(t_list *node, uint32_t flags)
{
	struct stat	statbuf;
	arg_t		*data = node->data;
	int			ret;

	ret = lstat(data->path, &statbuf);
	if (ret != 0)
	{
		write (2, "Error stat: ", 12);
		write(2, data->path, ft_strlen(data->path));
		write (2, "\n", 1);
		return (2);
	}

	// what after return ???
	ret = fill_arg_info(data);
	if (ret) return (ret);
	// what after return ???

	data->type = statbuf.st_mode & __S_IFMT;
	format_output(node, flags);

	return (ret);
}

int	check_arg_access(char *path, t_error *err)
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
	if ((sb.st_mode & __S_IFMT) == __S_IFDIR)
	{
		dir = opendir(path);
		if (dir == NULL)
			return (2);
		closedir(dir);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	int		ret;
	cmd_t	*ls;
	t_list	*tmp_lst;

	ls = malloc(sizeof(cmd_t));
	err_exit(!ls, "Malloc error", 2);
	if (argc < 1)
		return (1);
	ls->flags = 0;
	ls->args = NULL;
	ret = arg_parse(ls, argc, argv);
	if (ret) err_type_check(ls->err);
	if (ls->args == NULL)
		add_arg(ls, ".");
	tmp_lst = ls->args;
	while (tmp_lst)
	{
		ret = check_arg_access(((arg_t *)tmp_lst->data)->path, &ls->err);
		if (ret)
		{
			t_list *tmp2 = tmp_lst;
			printf ("Error on %s\n", ((arg_t *)tmp_lst->data)->path);
			tmp_lst = tmp_lst->next;
			delete_arg(&ls->args, tmp2);
			err_type_check(ls->err);
			continue ;
		}
		tmp_lst = tmp_lst->next;
	}
	sort_list(&ls->args, (ls->flags & LS_OPTION_t) ? SORT_BY_TIME : SORT_BY_NAME);
	tmp_lst = ls->args;
	while (tmp_lst)
	{
		ls->parent_path = ((arg_t *)tmp_lst->data)->path;
		ret = show_contents(tmp_lst, ls->flags);
		tmp_lst = tmp_lst->next;
	}
	// if (ret)
	// 	err_exit(ret, "error");
		
	ft_lstclear(&ls->args, &free_arg);
	free(ls);
	write (1, "\n", 1);
	return (0);
}
