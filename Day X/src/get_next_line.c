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
		if (read_size < BUFFER_SIZE)
			f.buffer[read_size] = '\0';
		if (read_size == 0 || read_size == -1)
			c = (char)read_size;
		else
			c = f.buffer[0];
	}
	f.cursor++;
	if (c == 0 || f.cursor == BUFFER_SIZE)
		f.cursor = 0;
	return (c);
}

int get_next_line(int fd, char **line)
{
	size_t	line_size;
	size_t	maxi_line;
	char	*tmp_line;
	char	c;

	line_size = 0;
	maxi_line = BUFFER_SIZE;
	if (fd < 0 || BUFFER_SIZE <= 0 || !line ||
	(*line = malloc(sizeof(char) * BUFFER_SIZE + 1)) == NULL)
		return (-1);
	while (((c = read_buffer(fd)) != '\n') && (c > 0))
	{
		(*line)[line_size++] = c;
		if (line_size == maxi_line)
		{
			maxi_line += BUFFER_SIZE;
			tmp_line = *line;
			if ((*line = malloc(sizeof(char) * maxi_line + 1)) == NULL)
				return (-1);
			ft_strncpy(*line, tmp_line, line_size);
			free(tmp_line);
		}
	}
	(*line)[line_size] = '\0';
	return ((c == 0 || c == -1) ? c : 1);
}
