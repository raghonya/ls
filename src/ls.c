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
int	show_contents(t_list *node, char *parent_path, uint32_t flags);

void return_code_check(int err_code, int exit_code)
{
	char	*err_message = "";

	switch(err_code)
	{
		case LS_ERR_INVALID_OPTION:
			//err_message = LS_ERR_MESSAGE_INVALID_OPTION; //pti poxvi code y flagi het
			//printf ("ooooo\n");
			break;
	}
	err_message = "yo";
	ft_putendl_fd(err_message, STDERR_FILENO);
	exit(exit_code); // nned to be changed
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
		// if (lstat(arg->path, &statbuf) != 0)
		// {
		// 	write (2, "Error stat\n", 11);
		// 	write(1, arg->path, ft_strlen(arg->path));
		// 	return (2);
		// }
		// switch (statbuf.st_mode & S_IFMT)
		// {
		// 	case FILETYPE:	break;
		// 	case BLK_DEV:	permissions[0] = 'b'; break;
		// 	case CHAR_DEV:	permissions[0] = 'c'; break;
		// 	case DIRECTORY:	permissions[0] = 'd'; break;
		// 	case FIFO:		permissions[0] = 'p'; break;
		// 	case LINK:		permissions[0] = 'l'; break;
		// 	case SOCK:		permissions[0] = 's'; break;
		// 	default:		permissions[0] = '?'; break;
		// }
		// char * tmp = ctime(&statbuf.st_mtime);
		// struct passwd	*usr_pwd;
		// struct group	*grp_pwd;

		// usr_pwd = getpwuid(statbuf.st_uid);
		// grp_pwd = getgrgid(statbuf.st_gid);

		// write(1, permissions, ft_strlen(permissions));
		// write(1, " ", 1);

		// char *tmp2;
		// tmp2 = ft_itoa((uintmax_t)statbuf.st_nlink);
		// write(1, tmp2, ft_strlen(tmp2));
		// write(1, " ", 1);
		// free(tmp2);

		// write(1, usr_pwd->pw_name, ft_strlen(usr_pwd->pw_name));
		// write(1, " ", 1);

		// write(1, grp_pwd->gr_name, ft_strlen(grp_pwd->gr_name));
		// write(1, " ", 1);

		// tmp2 = ft_itoa((intmax_t) statbuf.st_size);
		// write(1, tmp2, ft_strlen(tmp2));
		// write(1, " ", 1);
		// free(tmp2);

		// // tmp2 = ctime(&statbuf.st_mtime);
		// // write(1, tmp2, ft_strlen(tmp2) - 1);
		// printf ("%d\n", (intmax_t)statbuf.st_mtime);
		// write(1, " ", 1);

		// write(1, arg->name, ft_strlen(arg->name));
		// write(1, "\n", 1);
		// if ((statbuf.st_mode & S_IFMT) == LINK)
		// {
		// 	read_link(arg->path, &statbuf);
		// }
		// printf("%x", statbuf.st_mode); +++++++
		// printf(" %ju", (uintmax_t)statbuf.st_nlink); +++++++
		// printf (" %s", usr_pwd->pw_name); +++++++
		// printf (" %s", grp_pwd->gr_name); +++++++
		// printf(" %jd", (intmax_t) statbuf.st_size); +++++++
		// printf(" %s ", ctime(&statbuf.st_mtime)); 
		
		// printf("Mode: %x\n", statbuf.st_mode);
		// printf("Link count:               %ju\n", (uintmax_t) statbuf.st_nlink);
		// printf ("%s\n", usr_pwd->pw_arg->path);
		// printf ("%s\n", grp_pwd->gr_arg->path);
		// printf("Last file modification:   %s", ctime(&statbuf.st_mtime));
		// printf("Ownership:                UID=%ju   GID=%ju\n",
		// 		(uintmax_t) statbuf.st_uid, (uintmax_t) statbuf.st_gid);
		// printf("File size:                %jd bytes\n",
		// 		(intmax_t) statbuf.st_size);


		// call function for -l flag output
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
	t_list	*tmp;
	
	tmp = order;
	while (tmp)
	{
		print_name(tmp->data, tmp->next == NULL, flags);
		tmp = tmp->next;
	}
	ft_lstclear(&order, &free_arg);
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
	// char			*path = ((arg_t *)parent_node->data)->path;

	order = NULL;
	new_path = NULL;
	dir = opendir(path);
	if (!dir) return (2);
	// err_exit(!dir, "Dir error", 2);
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
	// else
	//	sort list like normal

	if (flags & LS_OPTION_r)
		order = reverse_list(order);
	if (LS_OPTION_R & flags)
		check_recursion(order, subdirs);
	closedir(dir);
	print_ordered(order, flags);
	write (1, "\n", 1);
	return (0);
}

int	open_dir(char *path, uint32_t flags)
{
	int		ret;
	t_list	*tmp;
	t_list	*subdirs;

	ret = check_dir_contents(&subdirs, path, flags);
	if (ret) return (ret);

	// tmp = subdirs;
	// while (tmp)
	// {
	// 	printf ("subdir %s\n", ((arg_t *)tmp->data)->path);
	// 	tmp = tmp->next;
	// }

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
	t_list		*subdirs;
	t_list		*tmp;

	ret = 0;
	subdirs = NULL;
	// printf ("node path: '%s'\n", data->path);
	if (data->type == S_IFDIR)
		ret = open_dir(data->path, flags);
	else
		print_name(data, 0, flags);
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
	data->type = statbuf.st_mode & S_IFMT;
	format_output(node, flags);

	return (ret);
}

int	check_arg_access(arg_t *arg)
{
	
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
	ls->arg = NULL;
	ret = arg_parse(ls, argc, argv);
	if (ret) return_code_check(ls->return_code, ret);
	if (ls->arg == NULL)
		add_arg(ls, ".");
	printf ("%d\n", ls->flags);

	tmp_lst = ls->arg;
	while (tmp_lst)
	{
		ls->parent_path = ((arg_t *)tmp_lst->data)->path;
		ret = show_contents(tmp_lst, ls->parent_path, ls->flags);
		tmp_lst = tmp_lst->next;
	}
	// if (ret)
		// err_exit(ret, "error");
		
	ft_lstclear(&ls->arg, &free_arg);

	free(ls);
	return (0);
}
