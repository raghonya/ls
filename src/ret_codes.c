#include "ls.h"

int		return_err_code(char *name, int code, int type, int trigger)
{

	int		err_code;
	char	*err_message;

	err_code = 0;
	err_message = "";
	ft_putstr_fd("ls: ", 2);
	switch (type)
	{
		case LS_ERR_INVALID_OPTION:
			write (2, "invalid option -- '", 19);
			ft_putstr_fd(name, 2);
			ft_putendl_fd ("'", 2);
			code = LS_ERR_RETURN_CODE_FATAL;
			// err_message = LS_ERR_MESSAGE_INVALID_OPTION; //pti poxvi code y flagi het
			// printf ("ooooo\n");
			// write (1, "ls: invalid")
			break;
		case LS_ERR_NO_SUCH_FILE_OR_DIRECTORY:
			write (2, "cannot access '", 15);
			ft_putstr_fd(name, 2);
			ft_putstr_fd ("'", 2);
			ft_putendl_fd (": No such file or directory", 2);
			break;
		case LS_ERR_PERMISSION_DENIED:
			// write (1, "", 1);
			break;
		case LS_ERR_NOT_ENOUGH_MEMORY:
			if (trigger)
			{
				err_message = "ls: fatal error: not enough memory";
				code = LS_ERR_RETURN_CODE_FATAL;
			}
			break;
		default:
			err_message = "Unknown error occurred";
			break;
	}
	return (code);
}

void err_type_check(t_error err)
{
	// char	*err_message = "";

	switch (0)
	{
		case LS_ERR_INVALID_OPTION:
			ft_putendl_fd(err.message, 1);
			exit(2);
			// err_message = LS_ERR_MESSAGE_INVALID_OPTION; //pti poxvi code y flagi het
			// printf ("ooooo\n");
			// write (1, "ls: invalid")
			break;
		case LS_ERR_NO_SUCH_FILE_OR_DIRECTORY:
			write (1, "", 1);
		case LS_ERR_PERMISSION_DENIED:
			write (1, "", 1);
	}

	// err_message = "yo";
	// ft_putendl_fd(err_message, STDERR_FILENO);
	// exit(exit_code); // need to be changed
}
