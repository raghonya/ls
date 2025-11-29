#include "ls.h"

void	free_arg(void *arg)
{
	free(((arg_t *)arg)->name);
	free(((arg_t *)arg)->path);
	free(arg);
}

arg_t*	create_arg(char *path, char *name)
{
	arg_t	*data;

	data = malloc(sizeof(arg_t));
	if (!data)
	{
		write(2, "Malloc error\n", 13);
		exit(2);
	}
	data->group = "";
	data->num_of_links = 0;
	data->owner = "";
	data->path = ft_strdup(path);
	data->name = ft_strdup(name);
	data->perm = 0;
	data->size = 0;
	data->time_last_modif = "";
	data->type = FILETYPE;

	return (data);
}

int	add_arg(cmd_t *ls, char *path)
{
	ft_lstadd_back(&ls->arg, ft_lstnew(create_arg(path, path)));
	return (0);
}

int	arg_parse(cmd_t *ls, int argc, char **argv)
{
	int		flag_len = 0;

	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-' && argv[i][1] != '-')
		{
			char *flag_str = argv[i] + 1;
			flag_len = ft_strlen(flag_str);
			for (int flag = 0; flag < flag_len; ++flag)
			{
				if (ls->flags == 0b11111) break ; // all flags are already set
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
		else
			add_arg(ls, argv[i]);
	}
	return (0);
}
