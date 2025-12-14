#include "ls.h"

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
	char			*tmp;
	struct stat		statbuf;
	struct passwd	*usr_pwd;
	struct group	*grp_pwd;

	if (lstat(arg->path, &statbuf) != 0)
	{
		write (2, "Error stat\n", 11);
		// write(1, arg->path, ft_strlen(arg->path));
		return (LS_ERR_RETURN_CODE_FATAL);
	}

	// type
	arg->type = statbuf.st_mode & __S_IFMT;

	// permissions
	arg->perm = check_permissions(statbuf.st_mode);

	// user and group
	usr_pwd = getpwuid(statbuf.st_uid);
	if (!usr_pwd) return (LS_ERR_RETURN_CODE_MINOR);
	grp_pwd = getgrgid(statbuf.st_gid);
	if (!grp_pwd) return (LS_ERR_RETURN_CODE_MINOR);
	arg->owner = usr_pwd->pw_name;
	arg->group = grp_pwd->gr_name;
	
	// link count
	arg->lnk_cnt = statbuf.st_nlink;

	// size
	arg->size = statbuf.st_size;

	// time
	arg->last_modif = statbuf.st_mtime;

	return (LS_ERR_RETURN_CODE_NO_ERROR);

}
