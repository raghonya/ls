#ifndef ERROR_H
# define ERROR_H

# define LS_ERR_PERMISSION_DENIED			1
# define LS_ERR_NOT_ENOUGH_MEMORY			2
# define LS_ERR_NO_SUCH_FILE_OR_DIRECTORY	3
# define LS_ERR_INVALID_OPTION 				4

typedef enum ret_code
{
	LS_ERR_RETURN_CODE_NO_ERROR = 0,
	LS_ERR_RETURN_CODE_MINOR,
	LS_ERR_RETURN_CODE_FATAL
} ret_code;

// typedef enum e_err_type
// {
// 	ERR_TYPE_NONE = 0,
// 	ERR_TYPE_INVALID_OPTION,
// 	ERR_TYPE_NO_SUCH_FILE_OR_DIRECTORY,
// 	ERR_TYPE_PERMISSION_DENIED,
// 	ERR_TYPE_NOT_ENOUGH_MEMORY
// }	t_err_type;

typedef struct s_error
{
	char		*message;
	ret_code	code;
	int			type;
} t_error;

#endif
