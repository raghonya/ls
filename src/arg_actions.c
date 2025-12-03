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
		exit(2);
	}
	ft_lstadd_back(&ls->arg, new_node);
	return (0);
}

int	arg_parse(cmd_t *ls, int argc, char **argv)
{
	int		flag_len = 0;

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
						ls->return_code = LS_ERR_INVALID_OPTION;
						return (2);
				}
			}
		}
		else
			add_arg(ls, argv[i]);
	}
	return (0);
}

char *calculate_permissions(mode_t mode)
{
	uint16_t	obj_perms;
	char		*mode_str;

	mode_str = ft_strdup("rwxrwxrwx");
	if (!mode_str)
		return (NULL);
	obj_perms = mode & 0777;
	for (int i = 0; i < 10; ++i)
	{
		if (!(obj_perms & (1 << i)))
		{
			mode_str[9 - i] = '-';
			// switch (i % 3)
			// {
			// 	case 0:
			// 		mode_str[9 - i] = 'x';
			// 		break;
			// 	case 1:
			// 		mode_str[9 - i] = 'w';
			// 		break;
			// 	case 2:
			// 		mode_str[9 - i] = 'r';
			// 		break;
			// }
		}
	}
	return (mode_str);
}

char	*check_permissions(mode_t mode)
{
	char	*permissions;
	char	*tmp;
	
	permissions = ft_strdup("-rwxrwxrwx");
	if (!permissions)
		return (NULL);
	switch (mode & S_IFMT)
	{
		case FILETYPE:	permissions[0] = '-';
		case BLK_DEV:	permissions[0] = 'b';
		case CHAR_DEV:	permissions[0] = 'c';
		case DIRECTORY:	permissions[0] = 'd';
		case FIFO:		permissions[0] = 'p';
		case LINK:		permissions[0] = 'l';
		case SOCK:		permissions[0] = 's';
		default:		permissions[0] = '?';
	}
	tmp = calculate_permissions(mode);
	if (!tmp)
		return (NULL);
	ft_memcpy(permissions + 1, tmp, ft_strlen(tmp));
	free(tmp);
	return (permissions);
}

int	fill_arg_info(arg_t *arg)
{
	int				ret;
	char			*tmp;
	struct stat		statbuf;
	struct passwd	*usr_pwd;
	struct group	*grp_pwd;
	char			permissions[11] = "----------";

	if (lstat(arg->path, &statbuf) != 0)
	{
		write (2, "Error stat\n", 11);
		write(1, arg->path, ft_strlen(arg->path));
		return (2);
	}

	// permissions
	arg->perm = check_permissions(statbuf.st_mode);

	// user and group
	usr_pwd = getpwuid(statbuf.st_uid);
	if (!usr_pwd) return (2);
	grp_pwd = getgrgid(statbuf.st_gid);
	if (!grp_pwd) return (2);
	arg->owner = usr_pwd->pw_name;
	arg->group = grp_pwd->gr_name;
	
	// link count
	arg->lnk_cnt = statbuf.st_nlink;

	// size
	arg->size = statbuf.st_size;

	// time
	arg->last_modif = statbuf.st_mtime;
	
	// tmp2 = ft_itoa((intmax_t) statbuf.st_size);
	// write(1, tmp2, ft_strlen(tmp2));
	// write(1, " ", 1);
	// free(tmp2);

	// tmp2 = ctime(&statbuf.st_mtime);
	// write(1, tmp2, ft_strlen(tmp2) - 1);
	// printf ("%d\n", (intmax_t)statbuf.st_mtime);
	// write(1, " ", 1);

	// write(1, arg->name, ft_strlen(arg->name));
	// write(1, "\n", 1);

}