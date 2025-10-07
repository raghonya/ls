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

//int	initialize(cmd_t *ls)
//{
//	ls->args = NULL;

//}

void add_elem_to_str(char *s, int len, char to_be_added)
{
	int i = 0;

	while (i < len && s[i])
		i++;
	if (i < len)
	{
		s[i] = to_be_added;
		s[i + 1] = 0;
	}
}

int	add_arg(cmd_t *ls, char *path)
{
	arg_t	*data;

	data = malloc(sizeof(arg_t));
	if (!data)
	{
		write(2, "Malloc error\n", 13);
		exit(2);
	}
	data->group = "";
	data->num_of_links = 0;
	data->owner = "";
	data->path = path;
	data->perm = 0;
	data->size = 0;
	data->time_last_modif = "";
	data->type = FILETYPE;
	ft_lstadd_back(&ls->arg, ft_lstnew(data));
	// minchev hishem
	return (0);
}

int	arg_parse(cmd_t *ls, int argc, char **argv)
{
	int		flag_len = 0;

	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			if (argv[i][1] != '-')
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
							ls->return_code = LS_ERR_INVALID_OPTION;
							return (1);
					}
				}
			}
		}
		else
			add_arg(ls, argv[i]);
	}
	return (0);
}

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

int	print_name(arg_t *data, uint32_t flags)
{
	if (data->path[0] == '.' && !(LS_OPTION_a & flags))
		return (0);
	if (LS_OPTION_l & flags)
	{
		//OPTION
		//write(1, )
	}
}

int	check_dir_contents(cmd_t *ls, char *parent_path, char *path)
{
	DIR				*dir;
	struct dirent	*elem;

	dir = opendir(path);
	if (!dir)
	{
		write(2, "Dir error\n", 10);
		exit(1);
	}
	//if (ls->flags & LS_OPTION_R)
	elem = readdir(dir);

	if (elem->d_type == DT_DIR)
	{
		
		if (ls->flags & LS_OPTION_R)
			write(1, ls->parent_path, ft_strlen(ls->parent_path));
		// show_contents(ls, );
	}
	while (elem != NULL)
	{
		//if (ls->flags & LS_OPTION_a)
			write (1, elem->d_name, ft_strlen(elem->d_name));
		// printf ("%ld\n%s\n%hu\n%u\n", elem->d_ino, elem->d_name, elem->d_reclen, elem->d_type);
		elem = readdir(dir);
		if (elem != NULL)
			write(1, " ", 1);
	}
	//printf ("%ld\n%s\n%hu\n%hu\n%u\n", elem->d_ino, elem->d_name, elem->d_namlen, elem->d_reclen, elem->d_type);
	closedir(dir);
}

int	show_contents(cmd_t *ls, t_list *node)
{
	struct stat	statbuf;
	arg_t		*data = node->data;

	//if (ls->flags & LS_OPTION_l)
	//{
	if (lstat(data->path, &statbuf) != 0)
	{
		write (2, "error stat\n", 11);
		exit(2);
	}
	// listxattr(data->path, )
	data->type = statbuf.st_mode & S_IFMT;
	//switch (data->type)
	//{
	//	case BLOCK_DEV:		printf("block device\n"); break;
	//	case LINK:			printf("symlink\n"); break;
	//	case CHARACTER_DEV:	printf("character device\n"); break;
	//	case DIRECTORY:		printf("directory\n"); break;
	//	case FILETYPE:		printf("regular file\n"); break;
	//	default:			printf("unknown?\n"); break;
	//}
	if ((statbuf.st_mode & S_IFMT) == S_IFDIR)
	{
		int ret = check_dir_contents(ls, data->path/*path*/);
	}
	//if ()
	//printf ("%d\n", statbuf.st_mode & S_IRUSR);
	//printf ("%d\n", statbuf.st_mode & S_IWUSR);
	//printf ("%d\n", statbuf.st_mode & S_IXUSR);
	//printf ("%d\n", statbuf.st_mode & S_IRGRP);
	//printf ("%d\n", statbuf.st_mode & S_IWGRP);
	//printf ("%d\n", statbuf.st_mode & S_IXGRP);
	//printf ("%d\n", statbuf.st_mode & S_IROTH);
	//printf ("%d\n", statbuf.st_mode & S_IWOTH);
	//printf ("%d\n", statbuf.st_mode & S_IXOTH);
//}
	//exit(1);
}

int	main(int argc, char **argv)
{
	cmd_t	*ls = malloc(sizeof(cmd_t));
	int		ret;

	if (argc < 1)
		return (1);
	//ls->used_flags[0] = 0;
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
		ret = show_contents(ls, tmp_arg);
		tmp_arg = tmp_arg->next;
	}
	if (ret)
		err_exit(ret, "error");

	//if (ls->flags & LS_OPTION_R)
	//	handle_recursion(ls);

	//tmp
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