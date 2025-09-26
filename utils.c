#include "ls.h"

void	err_exit(int ret_code, char *message)
{
	
}

off_t math_ceil(off_t divisible)
{
	off_t	divider;

	divider = 4096;
	
}

void	handle_l()
{
	
}

//void node_add_back(arg_t **list, arg_t *new)
//{
//	arg_t	*a;

//	if (!*list && new)
//	{
//		*list = new;
//		return ;
//	}
//	else if (!new)
//		return ;
//	a = *list;
//	while ((*list)->next)
//		*list = (*list)->next;
//	(*list)->next = new;
//	*list = a;
//}

//arg_t	*node_new(char *path)
//{
//	arg_t	*new;

//	new = malloc(sizeof(arg_t));
//	if (!new)
//		return (NULL);
//	new->type = FILETYPE;
//	new->num_of_links = 0;
//	new->owner = "";
//	new->group = "";
//	new->size = 0;
//	new->time_last_modif = "";
//	new->path = path;
//	new->next = NULL;
//	return (new);
//}
