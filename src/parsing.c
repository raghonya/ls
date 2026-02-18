#include "ls.h"

int	check_arg_access(char *path, int *is_dir, uint32_t opts)
{
	int			ret;
	struct stat	sb;
	DIR			*dir;

	ret = lstat(path, &sb);
	if (ret)
	{
		// print_error(path, LS_ERR_NO_SUCH_FILE_OR_DIRECTORY);
		ft_strcpy(g_err.name, path);
		g_err.type = LS_ERR_NO_SUCH_FILE_OR_DIRECTORY;
		return (LS_RETURN_CODE_FATAL);
	}

	*is_dir = ((sb.st_mode & __S_IFMT) == __S_IFDIR);
	if ((sb.st_mode & __S_IFMT) == __S_IFLNK \
	&& !(opts & LS_OPTION_l))
	{
		dir = opendir(path);
		if (dir == NULL)
			return (0);
		closedir(dir);
		*is_dir = 1;
	}
	// if (*is_dir)
	// {
	// 	dir = opendir(path);
	// 	if (dir == NULL)
	// 	{
	// 		// print_error(path, LS_ERR_PERMISSION_DENIED);
	// 		ft_strcpy(g_err.name, path);
	// 		g_err.type = LS_ERR_PERMISSION_DENIED;
	// 		return (LS_RETURN_CODE_FATAL);
	// 	}
	// 	closedir(dir);
	// }
	return (LS_RETURN_CODE_NO_ERROR);
}

int		non_option_arg_parse(cmd_t *ls, char *path)
{
	int		ret;
	int		is_dir;

	ret = check_arg_access(path, &is_dir, ls->opts);
	if (ret)
		return (ret);
	if (is_dir)
		ret = add_arg(&ls->dir_args, path);
	else
		ret = add_arg(&ls->file_args, path);
	return (ret);
}

int	arg_parse(cmd_t *ls, int argc, char **argv)
{
	int		ret;
	int		end_of_opts;
	// int		is_dir;
	char	*flag_str;

	end_of_opts = 0;
	for (int i = 1; i < argc; ++i)
	{
		switch (argv[i][0])
		{
			case '-':
				if (!end_of_opts && argv[i][1] == '-' && argv[i][2] == '\0')
				{
					end_of_opts = 1;
					continue ;
				}
				if (end_of_opts)
				{
					ret = non_option_arg_parse(ls, argv[i]);
					if (ret)
					{
						print_error(g_err.name, g_err.type);
						ls->arg_error = 1;
						continue ;
					}
				}
				else
				{
					flag_str = argv[i] + 1;
					for (int flag = 0; flag_str[flag]; ++flag)
					{
						if (ls->opts == 0b11111) break ; // all opts are already set
						switch (flag_str[flag])
						{
							case 'l': ls->opts |= LS_OPTION_l; break ;
							case 'R': ls->opts |= LS_OPTION_R; break ;
							case 'a': ls->opts |= LS_OPTION_a; break ;
							case 'r': ls->opts |= LS_OPTION_r; break ;
							case 't': ls->opts |= LS_OPTION_t; break ;
							default:
								ft_putstr_fd("ls: invalid option -- '", 2);
								ft_putchar_fd(flag_str[flag], 2);
								ft_putstr_fd("'\n", 2);
								ls->opt_error = 1;
								return (LS_RETURN_CODE_FATAL);
						}
					}
				}
				break ;
			break;
			default:
				ret = non_option_arg_parse(ls, argv[i]);
				if (ret)
				{
					print_error(g_err.name, g_err.type);
					ls->arg_error = 1;
					continue ;
				}
		}
	}
	// printf ("error: %d\n", ls->arg_error);
	if (ls->arg_error)
		return (LS_RETURN_CODE_FATAL);
	return (LS_RETURN_CODE_NO_ERROR);
}
