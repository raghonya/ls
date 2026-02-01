#include "libft.h"

char *ft_new_str_tolower(char *str)
{
	int 	i;
	char	*low_str;

	low_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!low_str)
		return (NULL);
	for (i = 0; str[i]; ++i)
		low_str[i] = ft_tolower(str[i]);
	low_str[i] = 0;
	return (low_str);
}
