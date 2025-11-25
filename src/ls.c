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

//int	handle_recursion(cmd_t *ls)
//{

//	return (0);
//}

int	print_name(char *name, int is_last, uint32_t flags)
{
	int	ret;

	ret = 0;
	if (name[0] == '.' && !(LS_OPTION_a & flags))
		return (0);
	if (LS_OPTION_l & flags)
	{
		// call function for -l flag output
	}
	else
	{
		write(1, name, ft_strlen(name));
		if (!is_last)
			write (1, " ", 1);
	}	
	return (ret);
}

int	expand_output(t_list *entity)
{
	struct passwd	*ent_pass;

	// ent_pass = getpwuid()
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
	while (elem != NULL)
	{
		if (flags & LS_OPTION_l)
			write (1, "\n", 1);
		if (elem->d_type == DT_DIR && flags & LS_OPTION_R \
			&& ft_strcmp(elem->d_name, ".") != 0 \
			&& ft_strcmp(elem->d_name, "..") != 0)
		{
			char *new_path, *tmp;
		
			new_path = ft_strjoin(path, "/");
			tmp = new_path;
			new_path = ft_strjoin(new_path, elem->d_name);
			free(tmp);
			// printf ("newpath: '%s'\n", new_path);
			ft_lstadd_back(subdirs, ft_lstnew(create_arg(new_path)));
		}

		ft_lstadd_back(&order, ft_lstnew(ft_strdup(elem->d_name)));
		if (LS_OPTION_l & flags)
		{
			// fill_info(ft_lstlast(*subdirs), statbuf);
		}


		elem = readdir(dir);
		// if (elem != NULL)
		// 	write(1, " ", 1);
	}
	// if (flags & LS_OPTION_t)
	// 	; sort list with time sorting
	// else if (flags & LS_OPTION_r)
		// sort list backwards
	// else
		// sort list like normal
	closedir(dir);
	// write(1, "\n", 1);
	// while ()
	t_list	*tmp;
	tmp = order;
	while (tmp)
	{
		// print from sorted list
		print_name(tmp->data, tmp->next == NULL, flags);
		tmp = tmp->next;
	}
	write (1, "\n\n", 2);
	
	return (0);
}

int	format_output(t_list **subdirs, struct stat *statbuf, t_list *parent_node, uint32_t flags)
{
	if ((statbuf->st_mode & S_IFMT) == S_IFDIR)
	{
		check_dir_contents(subdirs, statbuf, parent_node, flags);
	}
	else
	{
		// check_file();
	}
}

int	show_contents(t_list **node, char *parent_path, uint32_t flags)
{
	struct stat	statbuf;
	arg_t		*data = (*node)->data;
	int			ret;
	t_list		*subdirs;
	t_list		*tmp;

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
	// printf("Last status change:       %s", ctime(&statbuf.st_ctime));
	// printf("Last file access:         %s", ctime(&statbuf.st_atime));
	// printf("Last file modification:   %s", ctime(&statbuf.st_mtime));
	struct passwd	*usr_pwd;
	struct group	*grp_pwd;

	usr_pwd = getpwuid(statbuf.st_uid);
	printf ("%s\n", usr_pwd->pw_name);
	grp_pwd = getgrgid(statbuf.st_gid);
	printf ("%s\n", grp_pwd->gr_name);
	data->type = statbuf.st_mode & S_IFMT;
	format_output(&subdirs, &statbuf, *node, flags);
	if (data->type == S_IFDIR)
	{
		ret = check_dir_contents(&subdirs, &statbuf, *node, flags);
		// tmp = subdirs;
		// while (tmp)
		// {
		// 	printf ("subdirs: `%s`\n", ((arg_t *)tmp->data)->path);
		// 	tmp = tmp->next;
		// }
	}
	else
	{

	}
	while (subdirs != NULL)
	{
		show_contents(&subdirs, parent_path, flags);
		tmp = subdirs->next;
		// free all mallocs inside
		free(((arg_t *)subdirs->data)->path);
		free(subdirs->data);
		free(subdirs);
		subdirs = tmp;
	}
	//exit(1);
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
	//tmp
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