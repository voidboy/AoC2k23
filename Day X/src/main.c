#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <assert.h>
#include "get_next_line.h"
#include "split.h"

uint32_t part1(const char *line)
{
    return 0;
}

uint32_t part2(const char *line)
{
    return 0;
}

uint32_t aoc(uint32_t (*f)(const char *))
{
    uint32_t sum = 0;
    int fd = open("input.txt", O_RDONLY);
    char line[LINE_SIZE];

    while (get_next_line(fd, line) > 0)
        sum += f(line);

    return sum;
}

int main(void)
{
    assert(aoc(part1) == 0);
    assert(aoc(part2) == 0);
}
