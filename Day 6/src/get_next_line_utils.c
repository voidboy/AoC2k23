#include "get_next_line.h"

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	int i;

	i = 0;
	while (src[i] && n)
	{
		dest[i] = src[i];
		i++;
		n--;
	}
	if (n)
		while (n--)
			dest[i++] = '\0';
	return (dest);
}
