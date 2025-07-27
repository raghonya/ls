#include "ls.h"

//int	initialize(cmd_t *ls)
//{
//	ls->args = NULL;

//}

void add_elem_to_str(char *s, int len, char to_be_added)
{
	int i = 0;

	while (i < len && s[i])
		i++;
	if (i < len)
	{
		s[i] = to_be_added;
		s[i + 1] = 0;
	}
}

int	arg_parse(cmd_t *ls, int argc, char **argv)
{
	int		flag_len = 0;

	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			if (argv[i][1] != '-')
			{
				char *flag_str = argv[i] + 1;
				flag_len = ft_strlen(flag_str);
				for (int flag = 0; flag < flag_len; ++flag)
				{
					//printf ("%c\n", flag_str[flag]);
					//if (ft_strchr(LS_ALLOWED_FLAGS, flag_str[flag]) == NULL)
					//{
					//	ls->return_code = LS_ERR_INVALID_OPTION;
					//	return (1);
					//}
					//if (ft_strchr(ls->used_flags, flag_str[flag]) == NULL)
					//{
					//	add_elem_to_str(ls->used_flags, \
					//		STATIC_ARR_LENGTH(ls->used_flags, char), flag_str[flag]);
					//}
					if (ls->flags == 0b11111)
						break ; // all flags already set
					switch (flag_str[flag])
					{
						case 'l':
							ls->flags |= LS_OPTION_l;
							break ;
						case 'R':
							ls->flags |= LS_OPTION_R;
							break ;
						case 'a':
							ls->flags |= LS_OPTION_a;
							break ;
						case 'r':
							ls->flags |= LS_OPTION_r;
							break ;
						case 't':
							ls->flags |= LS_OPTION_t;
							break ;
						default:
							ls->return_code = LS_ERR_INVALID_OPTION;
							return (1);
					}
				}
			}
		}
	}
	return (0);
}

void return_code_check(int err_code, int exit_code)
{
	char	*err_message = "";

	switch(err_code)
	{
		case LS_ERR_INVALID_OPTION:
			//err_message = LS_ERR_MESSAGE_INVALID_OPTION; //pti poxvi code y flagi het
			//printf ("ooooo\n");
			break;
	}
	err_message = "yo";
	ft_putendl_fd(err_message, STDERR_FILENO);
	exit(exit_code); // nned to be changed
}

int	handle_recursion(cmd_t *ls)
{
	return (0);
}

int	main(int argc, char **argv)
{
	cmd_t	*ls = malloc(sizeof(cmd_t));
	int		ret;
	if (argc < 1)
		return (1);
	//ls->used_flags[0] = 0;
	ls->flags = 0;
	ret = arg_parse(ls, argc, argv);
	if (ret) return_code_check(ls->return_code, ret);
	printf ("%d\n", ls->flags);
	
	if (ls->flags & LS_OPTION_R)
		handle_recursion(ls);

	free(ls);
	return (0);
}

//int main()
//{
//	int a[8];

//	printf ("%d\n", STATIC_ARR_LENGTH(a, int));
//	//printf ("%d\n", sizeof(a) / sizeof(int));
//	//printf ("%d\n", sizeof(a));
//	//printf ("%d\n", sizeof(int));
//}