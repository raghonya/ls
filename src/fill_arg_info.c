#include "ls.h"

int	create_last_modif_time(time_t mod_time, arg_t *obj)
{
	int		ret;

	ret = set_full_datetime_flag(mod_time, &obj->is_full_datetime);
	if (ret)
		return (ret);
	

	return (mod_time);
}

char *calculate_permissions(mode_t mode)
{
	uint16_t	obj_perms;
	char		*mode_str;
	size_t		len;

	mode_str = ft_strdup("rwxrwxrwx");
	if (!mode_str)
		return (NULL);
	obj_perms = mode & 0777;
	len = ft_strlen(mode_str);
	for (int i = 0; i < len; ++i)
		if (!(obj_perms & (1 << i)))
			mode_str[len - i - 1] = '-';
	return (mode_str);
}

char	*check_permissions(mode_t mode)
{
	char	*permissions;
	char	*tmp;
	
	permissions = ft_strdup("-rwxrwxrwx");
	if (!permissions)
		return (NULL);
	switch (mode & __S_IFMT)
	{
		case FILETYPE:	permissions[0] = '-'; break ;
		case BLK_DEV:	permissions[0] = 'b'; break ;
		case CHAR_DEV:	permissions[0] = 'c'; break ;
		case DIRECTORY:	permissions[0] = 'd'; break ;
		case FIFO:		permissions[0] = 'p'; break ;
		case LINK:		permissions[0] = 'l'; break ;
		case SOCK:		permissions[0] = 's'; break ;
		default:		permissions[0] = '?'; break ;
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

	if (lstat(arg->path, &statbuf) != 0)
	{
		write (2, "Error stat\n", 11);
		// write(1, arg->path, ft_strlen(arg->path));
		return (LS_RETURN_CODE_FATAL);
	}
	// type
	arg->type = statbuf.st_mode & __S_IFMT;

	// permissions
	arg->perm = check_permissions(statbuf.st_mode);

	// user and group
	usr_pwd = getpwuid(statbuf.st_uid);
	grp_pwd = getgrgid(statbuf.st_gid);
	if (!usr_pwd || !grp_pwd)
	{
		// 
		return (LS_RETURN_CODE_MINOR);
	}
	arg->owner = ft_strdup(usr_pwd->pw_name);
	arg->group = ft_strdup(grp_pwd->gr_name);
	if (!arg->owner || !arg->group)
	{
		// 
		return (LS_RETURN_CODE_MINOR);
	}

	// link count
	arg->lnk_cnt = statbuf.st_nlink;

	// size
	arg->size = statbuf.st_size;
	if (arg->type == CHAR_DEV || arg->type == BLK_DEV)
		arg->size = statbuf.st_rdev;
	arg->blocks = statbuf.st_blocks;

	// time
	// arg->last_modif = statbuf.st_mtime;
	arg->last_modif = statbuf.st_mtime;
	ret = set_full_datetime_flag(statbuf.st_mtime, &arg->is_full_datetime);
	if (ret)
	{
		// 
		return (ret);
	}
	// printf ("filling info for: %s\n", arg->path);
	// printf ("stat size: %ld\n", statbuf.st_size);
	// printf ("stat blocks: %ld\n", statbuf.st_blocks);
	// printf ("stat mode: %o\n", statbuf.st_mode);
	// printf ("username: %s\n", getpwuid(statbuf.st_uid)->pw_name);
	// printf ("groupname: %s\n", getgrgid(statbuf.st_gid)->gr_name);
	return (LS_RETURN_CODE_NO_ERROR);

}
