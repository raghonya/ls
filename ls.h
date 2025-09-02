#ifndef LS_H
# define LS_H

# include <pwd.h>
# include <grp.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <dirent.h>
# include <sys/time.h>
# include <sys/xattr.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdint.h>


#include "libft.h"

//# define LS_ERR_MESSAGE_INVALID_OPTION(opt) "invalid option -- ''"

# define STATIC_ARR_LENGTH(arr, type) (size_t)(sizeof(arr) / sizeof(type))

# define LS_ALLOWED_FLAGS			"lRart"
# define LS_ALLOWED_FLAGS_COUNT		5

# define LS_OPTION_l				0b00001
# define LS_OPTION_R				0b00010
# define LS_OPTION_a				0b00100
# define LS_OPTION_r				0b01000
# define LS_OPTION_t				0b10000

//# define LS_ERR_MESSAGE_NO_SUCH_FILE_OR_DIRECTORY "ls: cannot access '': No such file or directory"

typedef enum error_codes_t
{
	LS_ERR_INVALID_OPTION = 1,
	LS_ERR_NO_SUCH_FILE_OR_DIRECTORY,
	LS_ERR_PERMISSION_DENIED,
} error_codes_t;

//typedef enum flag_bin_val_t
//{

//} flag_bin_val_t;


typedef enum arg_type_t
{
	FILETYPE = 0,
	DIRECTORY,
	LINK,
	BLOCK_DEV,
	CHARACTER_DEV
} arg_type_t;

typedef struct arg_t
{
	arg_type_t		type;
	size_t			num_of_links;
	char			*owner;
	char			*group;
	size_t			size;
	char			*time_last_modif;
	char			*path;
	struct arg_t	*next;
} arg_t;

typedef struct cmd_t
{
	int			return_code;
	uint32_t	flags;
	//int32_t	flags;
	//char		**args;
	arg_t		*resources;

} cmd_t;

void	err_exit(int ret_code, char *message);

arg_t	*node_new(char *path);
void	node_add_back(arg_t **list, arg_t *new);

#endif 