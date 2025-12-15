#include "libft.h"

char	*ft_strjoin_w_delim(char const *s1, char const *s2, char delim)
{
	char	*joined;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined = malloc(len1 + len2 + 2);
	if (!joined)
		return (NULL);
	ft_memcpy(joined, s1, len1);
	joined[len1] = delim;
	ft_memcpy(joined + len1 + 1, s2, len2);
	joined[len1 + 1 + len2] = '\0';

	return (joined);
}