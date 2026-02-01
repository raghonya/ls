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
# define LS_ALLOWED_OPTIONS_COUNT	5

# define LS_OPTION_l	(1 << 1)
# define LS_OPTION_R	(1 << 2)
# define LS_OPTION_a	(1 << 3)
# define LS_OPTION_r	(1 << 4)
# define LS_OPTION_t	(1 << 5)

# define SORT_BY_NAME	0
# define SORT_BY_TIME	1

extern t_error	g_err;
// extern int	g_err_type;
// extern int	g_err_code;

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

typedef struct t_info_max_lengths
{
	size_t	lnk_cnt_len;
	size_t	owner_len;
	size_t	group_len;
	size_t	major_len;
	size_t	minor_len;
	size_t	size_len;
	size_t	day_len;
} t_info_max_lengths;

// typedef struct arg_t
// {
// 	arg_type_t		type;
// 	char			*perm;
// 	nlink_t			lnk_cnt;
// 	char			*owner;
// 	char			*group;
// 	off_t			size;
// 	time_t			last_modif;
// 	char			*path;
// 	char			*name;
// 	off_t			blocks;
// 	int				is_full_datetime;
// } arg_t;

typedef struct arg_t
{
	arg_type_t		type;
	char			*perm;
	nlink_t			lnk_cnt;
	char			*owner;
	char			*group;
	off_t			size;
	time_t			last_modif;
	char			**time_split;
	char			*path;
	char			*name;
	off_t			blocks;
	int				is_full_datetime;
} arg_t;

typedef struct cmd_t
{
	uint32_t			opts;
	t_list				*args;
	t_list				*dir_args;
	t_list				*file_args;
	t_info_max_lengths	max_lengths;
	char				*parent_path;
	uint16_t			triggers;
	t_error				err;
	uint32_t			opt_error;
	char				arg_error;
} cmd_t;

// void	err_exit(int condition, char *message, int code);
int		print_ordered(t_list *order, t_info_max_lengths *max_lengths, uint32_t opts, int triggers);

// Argument related functions
int		create_arg(arg_t **data, char *path, char *name);
int		add_arg(t_list **lst, char *path);
void	free_arg(void *arg);
int		arg_parse(cmd_t *ls, int argc, char **argv);
int		fill_arg_info(arg_t *arg);
void	delete_arg(t_list **lst, t_list *node);

// Sorting functions
void	sort_with_opts(t_list **lst, uint32_t opts);
void	sort_list(t_list **lst, int by_time);
t_list	*reverse_list(t_list *head);

// Time and date related functions
int		set_full_datetime_flag(time_t input_time, int *is_full_datetime);
char	*format_time(time_t input_time, int is_full_datetime);

// Utils
// char	*str_to_lower(char *str);
void	slice_last_chars(char **str, char c);
char	*create_relative_path(char *path, char *name);
void	swap_ptrs(void **first, void **second);
void	print(t_list *lst);
// void	err_type_check(t_error err);
int		_err_code(int trigger);
void	print_error(char *name, int type);

// Tmp
void	print_all_info(arg_t *elem);
void	print(t_list *lst);

#endif
