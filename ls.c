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

int	nonflag_arg_parse(cmd_t *ls, char *path)
{
	node_add_back(&ls->resources, node_new(path));
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
			nonflag_arg_parse(ls, argv[i]);
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

int	handle_recursion(cmd_t *ls)
{

	return (0);
}

int	run_command(cmd_t *ls)
{
	struct stat	statbuf;

	if (lstat(ls->resources->path, &statbuf) != 0)
	{
		write (2, "error stat\n", 11);
	}
	if (ls->resources->path[ft_strlen(ls->resources->path) - 1] == '/')
	{
		printf("mejiny pti tpvi\n");
	}
	switch (statbuf.st_mode & S_IFMT)
	{
		case S_IFBLK:	printf("block device\n"); break;
		case S_IFLNK:	printf("symlink\n"); break;
		case S_IFCHR:	printf("character device\n"); break;
		case S_IFDIR:	printf("directory\n"); break;
		//case S_IFIFO:  printf("FIFO/pipe\n"); break;
		case S_IFREG:	printf("regular file\n"); break;
		//case S_IFSOCK: printf("socket\n"); break;
		default:		printf("unknown?\n"); break;
	}

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
	ls->resources = NULL;
	ret = arg_parse(ls, argc, argv);
	if (ret) return_code_check(ls->return_code, ret);
	printf ("%d\n", ls->flags);

	ret = run_command(ls);
	if (ret)
		err_exit(ret, "error");

	//if (ls->flags & LS_OPTION_R)
	//	handle_recursion(ls);

	//tmp
	while (ls->resources != NULL)
	{
		arg_t *tmp = ls->resources;
		ls->resources = ls->resources->next;
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