#include "get_next_line.h"

static inline char read_buffer(int fd)
{
	ssize_t				read_size;
	char				c;
	static	t_library	f;

	if (f.cursor)
		c = f.buffer[f.cursor];
	else
	{
		read_size = read(fd, f.buffer, BUFFER_SIZE);
		if (read_size > 0) {
			c = f.buffer[0];
            f.buffer[read_size] = '\0';
        } else {
			c = (char)read_size;
        }
	}
	f.cursor++;
	if (c == 0 || f.cursor == BUFFER_SIZE)
		f.cursor = 0;
	return (c);
}

int get_next_line(int fd, char *line)
{
	int  line_size;
	char c;

	line_size = 0;
	while (((c = read_buffer(fd)) != '\n') && (c > 0))
		line[line_size++] = c;
	line[line_size] = '\0';
	return ((c == 0 || c == -1) ? -1 : line_size);
}
