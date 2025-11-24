#include "ls.h"

void	lstadd_after(t_list *lst, t_list *new)
{
	if (!lst && new)
	{
		lst = new;
		return ;
	}
	else if (!new)
		return ;
	new->next = lst->next;
	lst->next = new;
}

int main(int argc, char **argv)
{
	t_list	*hello = NULL;
	
	ft_lstadd_back(&hello, ft_lstnew("barev"));
	ft_lstadd_back(&hello, ft_lstnew("dzez"));
	ft_lstadd_back(&hello, ft_lstnew("sireli"));
	ft_lstadd_back(&hello, ft_lstnew("erexaner"));
	ft_lstadd_back(&hello, ft_lstnew("paka"));

	t_list	*tmp = hello->next;


	lstadd_after(tmp, ft_lstnew("himar"));

	while (hello)
	{
		printf ("%s\n", (char *)hello->data);
		hello = hello->next;
	}

	return (0);
}

	//switch (data->type)
	//{
	//	case BLOCK_DEV:		printf("block device\n"); break;
	//	case LINK:			printf("symlink\n"); break;
	//	case CHARACTER_DEV:	printf("character device\n"); break;
	//	case DIRECTORY:		printf("directory\n"); break;
	//	case FILETYPE:		printf("regular file\n"); break;
	//	default:			printf("unknown?\n"); break;
	//}

	
	//if ()
	//printf ("%d\n", statbuf.st_mode & S_IRUSR);
	//printf ("%d\n", statbuf.st_mode & S_IWUSR);
	//printf ("%d\n", statbuf.st_mode & S_IXUSR);
	//printf ("%d\n", statbuf.st_mode & S_IRGRP);
	//printf ("%d\n", statbuf.st_mode & S_IWGRP);
	//printf ("%d\n", statbuf.st_mode & S_IXGRP);
	//printf ("%d\n", statbuf.st_mode & S_IROTH);
	//printf ("%d\n", statbuf.st_mode & S_IWOTH);
	//printf ("%d\n", statbuf.st_mode & S_IXOTH);