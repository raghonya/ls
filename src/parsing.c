#include "ls.h"

int	check_arg_access(char *path, t_error *err, int *is_dir)
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

	*is_dir = ((sb.st_mode & __S_IFMT) == __S_IFDIR);
	if (*is_dir)
	{
		dir = opendir(path);
		if (dir == NULL)
			return (2);
		closedir(dir);
	}
	return (0);
}

int		non_option_arg_parse(cmd_t *ls, char *path)
{
	int		ret;
	DIR		*dir;
	int		is_dir;

	ret = check_arg_access(path, &ls->err, &is_dir);
	if (ret)
	{
		printf ("Error on '%s'\n", path);
		// err_type_check(&ls->err);
		return (ret);
	}
	if (is_dir)
		ret = add_arg(&ls->dir_args, path);
	else
		ret = add_arg(&ls->file_args, path);
	// if (ret)
	// 	return (LS_ERR_RETURN_CODE_FATAL);
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
						printf ("Error in argparse: '%s'\n", argv[i]);
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
								ls->err.code = LS_ERR_INVALID_OPTION;
								return (LS_ERR_RETURN_CODE_FATAL);
						}
					}
				}
				break ;
			break;
			default:
				ret = non_option_arg_parse(ls, argv[i]);
				if (ret)
				{
					printf ("Error in argparse: '%s'\n", argv[i]);
					continue ;
				}
		}
	}
	return (0);
}
