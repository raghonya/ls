#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


// int main(int argc, char **argv)
// {
// 	struct stat ape;
// 	int a = stat(argv[1], &ape);
// 	printf ("%ld\n", ape.st_size);
// 	printf ("%ld\n", (ape.st_blocks / 2) - ((ape.st_blocks / 2) % 4));
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/xattr.h>

// int
// main(int argc, char *argv[])
// {
// 	char     *buf, *key, *val;
// 	ssize_t  buflen, keylen, vallen;

// 	if (argc != 2) {
// 		fprintf(stderr, "Usage: %s path\n", argv[0]);
// 		exit(EXIT_FAILURE);
// 	}

// 	/*
// 	* Determine the length of the buffer needed.
// 	*/
// 	buflen = listxattr(argv[1], NULL, 0);
// 	if (buflen == -1) {
// 		perror("listxattr");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (buflen == 0) {
// 		printf("%s has no attributes.\n", argv[1]);
// 		exit(EXIT_SUCCESS);
// 	}

// 	/*
// 	* Allocate the buffer.
// 	*/
// 	buf = malloc(buflen);
// 	if (buf == NULL) {
// 		perror("malloc");
// 		exit(EXIT_FAILURE);
// 	}

// 	/*
// 	* Copy the list of attribute keys to the buffer.
// 	*/
// 	buflen = listxattr(argv[1], buf, buflen);
// 	if (buflen == -1) {
// 		perror("listxattr");
// 		exit(EXIT_FAILURE);
// 	}

// 	/*
// 	* Loop over the list of zero terminated strings with the
// 	* attribute keys. Use the remaining buffer length to determine
// 	* the end of the list.
// 	*/
// 	key = buf;
// 	while (buflen > 0) {

// 		/*
// 		* Output attribute key.
// 		*/
// 		printf("%s: ", key);

// 		/*
// 		* Determine length of the value.
// 		*/
// 		vallen = getxattr(argv[1], key, NULL, 0);
// 		if (vallen == -1)
// 			perror("getxattr");

// 		if (vallen > 0) {

// 			/*
// 			* Allocate value buffer.
// 			* One extra byte is needed to append 0x00.
// 			*/
// 			val = malloc(vallen + 1);
// 			if (val == NULL) {
// 				perror("malloc");
// 				exit(EXIT_FAILURE);
// 			}

// 			/*
// 			* Copy value to buffer.
// 			*/
// 			vallen = getxattr(argv[1], key, val, vallen);
// 			if (vallen == -1) {
// 				perror("getxattr");
// 			} else {
// 				/*
// 				* Output attribute value.
// 				*/
// 				val[vallen] = 0;
// 				printf("%s", val);
// 			}

// 			free(val);
// 		} else if (vallen == 0) {
// 			printf("<no value>");
// 		}

// 		printf("\n");

// 		/*
// 		* Forward to next attribute key.
// 		*/
// 		keylen = strlen(key) + 1;
// 		buflen -= keylen;
// 		key += keylen;
// 	}

// 	free(buf);
// 	exit(EXIT_SUCCESS);
// }

#include <libft.h>
#include "../includes/ls.h"



// void	swap_args(arg_t **first, arg_t **second)

void	print(t_list *lst)
{
	while (lst)
	{
		// arg_t *data = lst->data;
		printf("'%s'\n", lst->data);
		lst = lst->next;
	}
}

char *str_to_lower(char *str)
{
	// size_t	len;
	int 	i;
	char	*low_str;

	// len = ft_strlen(str);
	low_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!low_str)
		return (NULL);
	for (i = 0; str[i]; ++i)
		low_str[i] = ft_tolower(str[i]);
	low_str[i] = 0;
	return (low_str);
}

void	swap_args(void **first, void **second)
{
	void *tmp;

	tmp = *first;
	*first = *second;
	*second = tmp;
}

char	*remove_symbols(char *str)
{
	char	*copy;

	copy = ft_strdup(str);
	if (!copy)
		return (copy);
	for (int i = 0; copy[i]; ++i)
	{
		if (!ft_isalnum(copy[i]))
		{
			for (int j = i; copy[j]; ++j)
				copy[j] = copy[j + 1];
			i--;
		}
	}
	return (copy);
}

int	compare_names(t_list *first, t_list *second)
{
	char *tmp;
	char *tmpc1;
	char *tmpc2;
	char *tmpc1_removed;
	char *tmpc2_removed;

	tmpc1 = str_to_lower(first->data);
	tmpc2 = str_to_lower(second->data);
	tmpc1_removed = remove_symbols(tmpc1);
	tmpc2_removed = remove_symbols(tmpc2);
	if (ft_strcmp(tmpc1_removed, tmpc2_removed) == 0)
	{
		if(ft_strcmp(tmpc1, tmpc2) > 0)
			swap_args(&first->data, &second->data);
	}
	else
	{
		// printf ("hamematvum en '%s' eb '%s'\n", tmpc1_removed, tmpc2_removed);
		if(ft_strcmp(tmpc1_removed, tmpc2_removed) > 0)
			swap_args(&first->data, &second->data);
	}
	free(tmpc1_removed);
	free(tmpc2_removed);
	free(tmpc1);
	free(tmpc2);
}

void sort_list(t_list **lst)
{
	t_list	*node;
	t_list	*temp;
	t_list	*temp2;

	temp = NULL;
	node = *lst;
	while (node != NULL)
	{
		temp = node;
		temp2 = temp->next;
		while (temp2 != NULL)
		{
			compare_names(temp, temp2);
			temp2 = temp2->next;
		}
		node = node->next;
	}
}

void	read_dir_file_names(t_list **lst)
{
	struct dirent *de;
    DIR *dr = opendir("../symbols_test");
    if (dr == NULL)
    {
        printf("Could not open current directory" );
    }
    while ((de = readdir(dr)) != NULL)
	{
		if (de->d_name[0] != '.')
			ft_lstadd_back(lst, ft_lstnew(ft_strdup(de->d_name)));
	}
    closedir(dr);    
}

int main()
{
	t_list *ban = NULL;

	// char *esim = malloc(20);
	// ft_memcpy(esim, "_8/516as.tf", ft_strlen("_..8/516as.tf."));
	// printf ("%s\n", remove_symbols(esim));

	// printf ("%p\n", ban->next->next);
	// checkref(&ban, ban->next->next);
	//    !world #hello  &hello   hello Make make test.txt
	// ft_lstadd_back(&ban, ft_lstnew("testt.at"));
	// ft_lstadd_back(&ban, ft_lstnew("7a1"));
	// ft_lstadd_back(&ban, ft_lstnew("hello"));
	// ft_lstadd_back(&ban, ft_lstnew("walrd"));
	// ft_lstadd_back(&ban, ft_lstnew("test.axt"));
	// // ft_lstadd_back(&ban, ft_lstnew(" hello"));
	// // ft_lstadd_back(&ban, ft_lstnew("4c8"));
	// // ft_lstadd_back(&ban, ft_lstnew("#hello"));
	// ft_lstadd_back(&ban, ft_lstnew("!hello"));
	// ft_lstadd_back(&ban, ft_lstnew("Make"));
	// ft_lstadd_back(&ban, ft_lstnew("'hello"));
	// ft_lstadd_back(&ban, ft_lstnew("$hello"));
	// ft_lstadd_back(&ban, ft_lstnew("&hello"));
	// ft_lstadd_back(&ban, ft_lstnew("make"));
	// ft_lstadd_back(&ban, ft_lstnew("!world"));
	// ft_lstadd_back(&ban, ft_lstnew("test.txt"));
	// printf ("i sfte\n\n");
	// print(ban);
	// printf ("\ni sfte avart\n");
	read_dir_file_names(&ban);
	sort_list(&ban);
	// printf ("aaa\n\n");
	print(ban);
	// printf ("aaa\n");
	// char perms[11] = "1234567890";
	// char bla[11];

	// ft_memcpy(bla, perms, ft_strlen(perms));
	// printf ("%s\n", bla);
}
