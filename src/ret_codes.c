#include "ls.h"
#include <errno.h>

int		_err_code(int trigger)
{
	return (trigger & RECURSION_FUNC \
		? LS_RETURN_CODE_MINOR \
		: LS_RETURN_CODE_FATAL);
}

void	print_with_quotes(char *start, char *name, int colon)
{
	// write (1, "'", 1)
	ft_putstr_fd(start, 2);
	ft_putchar_fd('\'', 2);
	ft_putstr_fd(name, 2);
	ft_putchar_fd('\'', 2);
	if (colon)
		ft_putstr_fd(": ", 2);
}

void	print_error(char *name, int type)
{
	ft_putstr_fd("ls: ", 2);
	switch (type)
	{
		// case LS_ERR_INVALID_OPTION:
		// 	print_with_quotes("invalid option -- ", name, 0);
		// 	ft_putendl_fd ("", 2);
		// 	break;
		case LS_ERR_NO_SUCH_FILE_OR_DIRECTORY:
			print_with_quotes("cannot access ", name, 1);
			perror("");
			break;
		case LS_ERR_PERMISSION_DENIED:
			print_with_quotes("cannot open directory ", name, 1);
			perror("");
			break;
		case LS_ERR_NOT_ENOUGH_MEMORY:
			print_with_quotes("fatal error: ", name, 1);
			perror("");
			break;
		default:
			print_with_quotes("Unknown error occurred ", name, 1);
			perror("");
			break;
	}
}