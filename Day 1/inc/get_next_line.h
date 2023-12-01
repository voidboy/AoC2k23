#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# define BUFFER_SIZE 0x1000
# include <stdlib.h>
# include <unistd.h>

typedef struct	s_library {
	char			buffer[BUFFER_SIZE];
	unsigned int	cursor;
}				t_library;

int				get_next_line(int fd, char **line);
char			*ft_strncpy(char *dest, const char *src, size_t n);
#endif
