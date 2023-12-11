#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "get_next_line.h"
#include "split.h"

uint64_t part1(void)
{
    uint64_t sum = 0;
    int fd = open("input.txt", O_RDONLY);
    char line[LINE_SIZE];

    while (get_next_line(fd, line) > 0) {
        ;
    }

    return sum;
}

uint64_t part2(void)
{
    uint64_t sum = 0;
    int fd = open("input.txt", O_RDONLY);
    char line[LINE_SIZE];

    while (get_next_line(fd, line) > 0) {
        ;
    }

    return sum;
}

int main(void)
{
    assert(part1() == 0);
    assert(part2() == 0);
}
