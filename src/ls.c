#include "ls.h"
#include <errno.h>
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

// void add_elem_to_str(char *s, int len, char to_be_added)
// {
// 	int i = 0;

// 	while (i < len && s[i])
// 		i++;
// 	if (i < len)
// 	{
// 		s[i] = to_be_added;
// 		s[i + 1] = 0;
// 	}
// }
// int	show_contents(t_list **node, char *parent_path, uint32_t flags);

void	print(t_list *lst)
{
	while (lst)
	{
		arg_t *data = lst->data;
		printf("%s\n", data->name);
		lst = lst->next;
	}
}

int	show_contents(t_list *node, char *parent_path, uint32_t flags);

void err_type_check(t_error err)
{
	// char	*err_message = "";

	switch (err.type)
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
	// exit(exit_code); // nned to be changed
}

int		read_link(char *link, struct stat *statbuf)
{

	char	*buf;
	ssize_t	bufsiz, nbytes;

	bufsiz = statbuf->st_size + 1;

	/* Some magic symlinks under (for example) /proc and /sys
		report 'st_size' as zero. In that case, take PATH_MAX as
		a "good enough" estimate. */

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

	/* Print only 'nbytes' of 'buf', as it doesn't contain a terminating
		null byte ('\0'). */
	printf("'%s' points to '%.*s'\n", link, (int) nbytes, buf);

	/* If the return value was equal to the buffer size, then
		the link target was larger than expected (perhaps because the
		target was changed between the call to lstat() and the call to
		readlink()). Warn the user that the returned target may have
		been truncated. */

	// if (nbytes == bufsiz)
	// 	printf("(Returned buffer may have been truncated)\n");

	free(buf);
}

int	print_name(arg_t *arg, int is_last, uint32_t flags)
{
	int			ret;
	struct stat	statbuf;
	char		permissions[11] = "-rwxrwxrwx";

	ret = 0;
	if (!(LS_OPTION_a & flags) && arg->name[0] == '.')
		return (0);
	if (LS_OPTION_l & flags)
	{

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
	// write(1, "\n", 1);
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
		// printf("elem name: '%s'\n", elem->d_name);
		t_list *tmp_lst;
		tmp_lst = ft_lstnew(create_arg(new_path, elem->d_name));
		err_exit(!tmp_lst, "Malloc error", 2);
		((arg_t *)tmp_lst->data)->type = elem->d_type;
		ft_lstadd_back(&order, tmp_lst);
		free(new_path);
		elem = readdir(dir);
	}

	// sort
	
	if (flags & LS_OPTION_t)
	{
		//sort list with time sorting
		;
	}
	else
	{
		printf("just order\n");
		sort_list(&order);
	}

	if (flags & LS_OPTION_r)
		order = reverse_list(order);
	if (flags & LS_OPTION_R)
		check_recursion(order, subdirs);
	closedir(dir);
	print_ordered(order, flags);
	ft_lstclear(&order, &free_arg);

	// write (1, "\n", 1);
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

int	show_contents(t_list *node, char *parent_path, uint32_t flags)
{
	struct stat	statbuf;
	arg_t		*data = node->data;
	int			ret;

	// printf ("in show contents, data path is '%s'\n", data->path);
	// printf ("node path: '%s'\n", data->path);
	ret = lstat(data->path, &statbuf);
	// dprintf (2, "data->path: '%s' %d\n", data->path, ret);
	if (ret != 0)
	{
		write (2, "Error stat: ", 12);
		write(2, data->path, ft_strlen(data->path));
		write (2, "\n", 1);
		// exit(2);
		return (2);
	}

	// ret = fill_arg_info(data);
	
	// printf ("perms: %o\n", statbuf.st_mode & 0777);
	// calculate_permissions(statbuf.st_mode & 0777);
	// printf ("blksize: %ld\n", statbuf.st_blocks);
	// printf ("%ld\n", statbuf.st_size);
	// printf ("%d\n", statbuf.st_uid);
	// printf ("%u\n", statbuf.st_atimensec);

	// printf("Last file modification:   %s", ctime(&statbuf.st_mtime));
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
	printf ("%d\n", ls->flags);

	// amen folderi vra kancel opendir, heto yst dra stugel permissionnery u
	// ancnelov sax argumentneri vrov, het gal show_contents kamchel normalneri vra
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
		// ls->parent_path = ((arg_t *)tmp_lst->data)->path;
	}
	sort_list(&ls->args);
	// printf ("what\n");
	// print(ls->args);
	// printf ("what\n");
	tmp_lst = ls->args;
	while (tmp_lst)
	{
		ls->parent_path = ((arg_t *)tmp_lst->data)->path;
		ret = show_contents(tmp_lst, ls->parent_path, ls->flags);
		tmp_lst = tmp_lst->next;
	}
	// if (ret)
	// 	err_exit(ret, "error");
		
	ft_lstclear(&ls->args, &free_arg);
	free(ls);
	write (1, "\n", 1);
	return (0);
}
