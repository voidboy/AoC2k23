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
        int64_t extrapolate[64][256] = {0};
        char **history = split(line, ' ');
        size_t sequence_length = 0;
        for (;history[sequence_length]; sequence_length++)
            extrapolate[0][sequence_length] = atoll(history[sequence_length]);
        size_t step = 1, current_length = sequence_length - 1;
        while (1) {
            bool finish_him = true;
            for (size_t i = 0; i < current_length; i++) {
                extrapolate[step][i] = extrapolate[step - 1][i + 1] - extrapolate[step - 1][i];
                extrapolate[step][i + 1] = 0;
                if (extrapolate[step][i] != 0) { finish_him = false; }
            }
            if (finish_him) { break ; } else { current_length--; step++; }
        }
        for (int i = step - 1; i >= 0; i--) {
            extrapolate[i][sequence_length - i] = extrapolate[i][sequence_length - (i + 1)]
                + extrapolate[i + 1][sequence_length - (i + 1)];
        }
        sum += extrapolate[0][sequence_length];
    }

    return sum;
}

uint64_t part2(void)
{
    int64_t sum = 0;
    int fd = open("input.txt", O_RDONLY);
    char line[LINE_SIZE];

    while (get_next_line(fd, line) > 0) {
        int64_t last = 0, extrapolate[64][256] = {0};
        char **history = split(line, ' ');
        size_t sequence_length = 0;
        for (;history[sequence_length]; sequence_length++)
            extrapolate[0][sequence_length] = atoll(history[sequence_length]);
        size_t step = 1, current_length = sequence_length - 1;
        while (1) {
            bool finish_him = true;
            for (size_t i = 0; i < current_length; i++) {
                extrapolate[step][i] = extrapolate[step - 1][i + 1] - extrapolate[step - 1][i];
                extrapolate[step][i + 1] = 0;
                if (extrapolate[step][i] != 0) { finish_him = false; }
            }
            if (finish_him) { break ; } else { current_length--; step++; }
        }
        for (int i = step - 1; i >= 0; i--)
            last = extrapolate[i][0] - last;
        sum += last;
    }

    return sum;
}

int main(void)
{
    assert(part1() == 1904165718);
    assert(part2() == 964);
}
