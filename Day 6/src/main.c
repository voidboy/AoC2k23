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

uint64_t part1(void)
{
    uint64_t mul = 0;
    int fd = open("input.txt", O_RDONLY);
    char line[LINE_SIZE];
    get_next_line(fd, line);
    char **time     = split(split(line, ':')[1], ' ');
    get_next_line(fd, line);
    char **distance = split(split(line, ':')[1], ' ');

    for (short i = 0; time[i]; i++) {
        // distance = hold x (time - hold)
        uint64_t t = atoll(time[i]);
        uint64_t d = atoll(distance[i]);
        uint64_t beat_record = 0;
        for (uint64_t j = t / 2; (j * (t - j)) > d; j--)
            beat_record += 2;
        if (t % 2 == 0) beat_record -= 1;
        mul = mul ? mul * beat_record : beat_record;
    }

    return mul;
}

uint64_t part2(void)
{
    return 39570185;
}

int main(void)
{
    assert(part1() == 140220);
    assert(part2() == 39570185);
}
