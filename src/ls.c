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
int	show_contents(t_list **node, char *parent_path, uint32_t flags);

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

int	print_name(arg_t *arg, int is_last, uint32_t flags)
{
	int			ret;
	struct stat	statbuf;
	char		permissions[11] = "-rwxrwxrwx";

	ret = 0;
	// printf ("name: %s\n", arg->path);
	if (arg->name[0] == '.' && !(LS_OPTION_a & flags))
		return (0);
	if (LS_OPTION_l & flags)
	{
		if (lstat(arg->path, &statbuf) != 0)
		{
			write (2, "Error stat\n", 11);
			write(1, arg->path, ft_strlen(arg->path));
			return (2);
		}
		switch (statbuf.st_mode & S_IFMT)
		{
			case FILETYPE:	break;
			case BLK_DEV:	permissions[0] = 'b'; break;
			case CHAR_DEV:	permissions[0] = 'c'; break;
			case DIRECTORY:	permissions[0] = 'd'; break;
			case FIFO:		permissions[0] = 'p'; break;
			case LINK:		permissions[0] = 'l'; break;
			case SOCK:		permissions[0] = 's'; break;
			default:		permissions[0] = '?'; break;
		}
		char * tmp = ctime(&statbuf.st_mtime);
		struct passwd	*usr_pwd;
		struct group	*grp_pwd;

		usr_pwd = getpwuid(statbuf.st_uid);
		grp_pwd = getgrgid(statbuf.st_gid);

		write(1, permissions, ft_strlen(permissions));
		write(1, " ", 1);

		char *tmp2;
		tmp2 = ft_itoa((uintmax_t)statbuf.st_nlink);
		write(1, tmp2, ft_strlen(tmp2));
		write(1, " ", 1);
		free(tmp2);

		write(1, usr_pwd->pw_name, ft_strlen(usr_pwd->pw_name));
		write(1, " ", 1);

		write(1, grp_pwd->gr_name, ft_strlen(grp_pwd->gr_name));
		write(1, " ", 1);

		tmp2 = ft_itoa((intmax_t) statbuf.st_size);
		write(1, tmp2, ft_strlen(tmp2));
		write(1, " ", 1);
		free(tmp2);

		tmp2 = ctime(&statbuf.st_mtime);
		write(1, tmp2, ft_strlen(tmp2) - 1);
		write(1, " ", 1);

		write(1, arg->name, ft_strlen(arg->name));
		write(1, "\n", 1);
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
		write(1, arg->path, ft_strlen(arg->path));
		if (!is_last)
			write (1, " ", 1);
	}	
	return (ret);
}

void	slice_last_chars(char **str, char c)
{
	int	len;

	len = ft_strlen(*str);
	for (int i = len - 1; i >= 0 && *(*str + i) == c; --i)
		*(*str + i) = 0;
}

int	check_dir_contents(t_list **subdirs, struct stat *statbuf, t_list *parent_node, uint32_t flags)
{
	DIR				*dir;
	struct dirent	*elem;
	t_list			*order;
	char			*path = ((arg_t *)parent_node->data)->path;

	order = NULL;
	dir = opendir(path);
	if (!dir)
	{
		write(2, "Dir error\n", 10);
		exit(1);
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
		// if (flags & LS_OPTION_l)
		// 	write (1, "\n", 1);
		char *new_path, *tmp;
		new_path = ft_strjoin(path, "/");
		tmp = new_path;
		new_path = ft_strjoin(new_path, elem->d_name);
		free(tmp);
		if ((flags & LS_OPTION_R) && elem->d_type == DT_DIR \
		&& ft_strcmp(elem->d_name, ".") != 0 \
		&& ft_strcmp(elem->d_name, "..") != 0)
		{
			ft_lstadd_back(subdirs, ft_lstnew(create_arg(new_path, elem->d_name)));
		}

		ft_lstadd_back(&order, ft_lstnew(create_arg(new_path, elem->d_name)));
		// ((arg_t *)(ft_lstlast(order)->data))->type = statbuf->st_mode & S_IFMT;
		elem = readdir(dir);
	}
	// if (flags & LS_OPTION_t)
	// 	; sort list with time sorting
	// else if (flags & LS_OPTION_r)
		// sort list backwards
	// else
		// sort list like normal

	closedir(dir);
	t_list	*tmp;
	while (order)
	{
		// print from sorted list
		print_name(order->data, order->next == NULL, flags);
		tmp = order;
		order = order->next;
		// free(((arg_t *)tmp->data)->path);
		free(tmp->data);
		free(tmp);
	}
	write (1, "\n", 1);
	
	return (0);
}

int	format_output(t_list **subdirs, struct stat *statbuf, t_list *parent_node, uint32_t flags)
{
	int			ret;
	arg_t		*data = ((arg_t *)parent_node->data);
	t_list		*tmp;

	ret = 0;
	if (data->type == S_IFDIR)
	{
		ret = check_dir_contents(subdirs, statbuf, parent_node, flags);
		while (*subdirs != NULL)
		{
			// tmp = *subdirs;
			// while (tmp)
			// {
			// 	printf ("%s\n", ((arg_t *)tmp->data)->path);
			// 	tmp = tmp->next;
			// }
			show_contents(subdirs, data->path, flags);
			tmp = (*subdirs)->next;
			// free all mallocs inside
			free(((arg_t *)(*subdirs)->data)->path);
			free((*subdirs)->data);
			free(*subdirs);
			(*subdirs) = tmp;
		}
	}
	else
	{
		print_name(data, 0, flags);
	}
}

int	show_contents(t_list **node, char *parent_path, uint32_t flags)
{
	struct stat	statbuf;
	arg_t		*data = (*node)->data;
	int			ret;
	t_list		*subdirs;

	subdirs = NULL;
	// printf ("in show contents, data path is '%s'\n", data->path);
	if (lstat(data->path, &statbuf) != 0)
	{
		write (2, "Error stat\n", 11);
		write(1, data->path, ft_strlen(data->path));
		exit(2);
	}

	// printf ("%lu\n", statbuf.st_atim);
	// printf ("%ld\n", statbuf.st_blksize);
	// printf ("%ld\n", statbuf.st_size);
	// printf ("%d\n", statbuf.st_uid);
	// printf ("%u\n", statbuf.st_atimensec);

	// listxattr(data->path, )
	// printf("Mode:                     %x (octal)\n",
	// 		statbuf.st_mode);
	// printf("Link count:               %ju\n", (uintmax_t) statbuf.st_nlink);
	// printf("Ownership:                UID=%ju   GID=%ju\n",
	// 		(uintmax_t) statbuf.st_uid, (uintmax_t) statbuf.st_gid);
	// printf("File size:                %jd bytes\n",
	// 		(intmax_t) statbuf.st_size);
	
	// printf("Last file modification:   %s", ctime(&statbuf.st_mtime));
	data->type = statbuf.st_mode & S_IFMT;
	format_output(&subdirs, &statbuf, *node, flags);

	return (ret);
}

int	main(int argc, char **argv)
{
	cmd_t	*ls = malloc(sizeof(cmd_t));
	int		ret;

	if (argc < 1)
		return (1);
	ls->flags = 0;
	ls->arg = NULL;
	ret = arg_parse(ls, argc, argv);
	if (ret) return_code_check(ls->return_code, ret);
	if (ls->arg == NULL)
		add_arg(ls, ".");
	printf ("%d\n", ls->flags);


	t_list	*tmp_arg = ls->arg;
	while (tmp_arg)
	{
		ls->parent_path = ((arg_t *)tmp_arg->data)->path;
		ret = show_contents(&tmp_arg, ls->parent_path, ls->flags);
		tmp_arg = tmp_arg->next;
	}
	// if (ret)
		// err_exit(ret, "error");
	while (ls->arg != NULL)
	{
		t_list *tmp = ls->arg;
		ls->arg = ls->arg->next;
		free(tmp->data);
		free(tmp);
	}

	free(ls);
	return (0);
}

//int main()
//{
//	char *a = "";
//	int i;
//	a = malloc(sizeof(char) * 8 + 1);
//	for (i = 0; i < 8; ++i)
//		a[i] = i + 48;
//	a[i] = 0;
//	printf ("%s\n", a);
//	//printf ("%d\n", sizeof(a) / sizeof(int));
//	//printf ("%d\n", sizeof(a));
//	//printf ("%d\n", sizeof(int));
//}