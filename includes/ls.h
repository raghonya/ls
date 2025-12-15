#ifndef LS_H
# define LS_H

# include <pwd.h>
# include <grp.h>
# include <time.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <dirent.h>
# include <sys/time.h>
# include <sys/xattr.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdint.h>

# include "libft.h"
# include "error.h"
# include "triggers.h"

# define STATIC_ARR_LENGTH(arr, type) (size_t)(sizeof(arr) / sizeof(type))

# define LS_ALLOWED_OPTIONS			"lRart"
# define LS_ALLOWED_OPTIONS_COUNT		5

# define LS_OPTION_l	0b00001
# define LS_OPTION_R	0b00010
# define LS_OPTION_a	0b00100
# define LS_OPTION_r	0b01000
# define LS_OPTION_t	0b10000

# define SORT_BY_NAME	0
# define SORT_BY_TIME	1

//# define LS_ERR_MESSAGE_NO_SUCH_FILE_OR_DIRECTORY "ls: cannot access '': No such file or directory"

typedef enum arg_type_t
{
	FILETYPE	= __S_IFREG,
	DIRECTORY	= __S_IFDIR,
	LINK		= __S_IFLNK,
	BLK_DEV		= __S_IFBLK,
	CHAR_DEV	= __S_IFCHR,
	FIFO		= __S_IFIFO,
	SOCK		= __S_IFSOCK
} arg_type_t;

typedef struct arg_t
{
	arg_type_t		type;
	char			*perm;
	nlink_t			lnk_cnt;
	char			*owner;
	char			*group;
	off_t			size;
	time_t			last_modif;
	char			*path;
	char			*name;
	off_t			blocks;
	int				is_full_datetime;
} arg_t;

typedef struct cmd_t
{
	uint32_t		opts;
	t_list			*args;
	t_list			*dir_args;
	t_list			*file_args;
	char			*parent_path;
	uint16_t		triggers;
	t_error			err;
} cmd_t;

void	err_exit(int condition, char *message, int code);
int		create_arg(arg_t **data, char *path, char *name);
int		add_arg(t_list **lst, char *path);
void	free_arg(void *arg);
int		arg_parse(cmd_t *ls, int argc, char **argv);
int		fill_arg_info(arg_t *arg);
void	delete_arg(t_list **lst, t_list *node);
// void	sort_list(t_list **lst);
void	sort_with_opts(t_list **lst, uint32_t opts);
void	sort_list(t_list **lst, int by_time);

void	swap_ptrs(void **first, void **second);
char	*str_to_lower(char *str);
void	slice_last_chars(char **str, char c);
char	*create_relative_path(char *path, char *name);
t_list	*reverse_list(t_list *head);

// Time and date related functions
int		set_full_datetime_flag(time_t input_time, int *is_full_datetime);
char	*format_time(time_t input_time, int is_full_datetime);

void	print(t_list *lst);

void	err_type_check(t_error err);

#endif
