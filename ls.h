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

#include "libft.h"

# define LS_ERR_MESSAGE_INVALID_OPTION(opt) "invalid option -- ''"

# define STATIC_ARR_LENGTH(arr, type) (size_t)(sizeof(arr) / sizeof(type))

# define LS_ALLOWED_FLAGS "lRart"
# define LS_ALLOWED_FLAGS_COUNT 5

typedef enum error_codes_t
{
	LS_ERR_INVALID_OPTION = 1,
	LS_ERR_NO_SUCH_FILE_OR_DIRECTORY
} error_codes_t;

typedef enum flag_bin_val_t
{
	LS_OPTION_l = 0b00001,
	LS_OPTION_R = 0b00010,
	LS_OPTION_a = 0b00100,
	LS_OPTION_r = 0b01000,
	LS_OPTION_t = 0b10000,

} flag_bin_val_t;
typedef struct cmd_t 
{
	//char	used_flags[LS_ALLOWED_FLAGS_COUNT + 1];
	int		return_code;
	char	**paths;
	int32_t	flags;
	//int		is_dir;
	//int32_t	flags;
	char	**args;

} cmd_t;



#endif 