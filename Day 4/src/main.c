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
#define LINES 213

uint64_t part1(void)
{
    uint64_t sum = 0;
    int fd = open("input.txt", O_RDONLY);
    char line[LINE_SIZE];

    while (get_next_line(fd, line) > 0) {
        char **foo = split(line,    ':');
        char **bar = split(foo[1],  '|');
        char **win_numbers = split(bar[0], ' ');
        char **the_numbers = split(bar[1], ' ');
        int  match = 0;
        for (int i = 0; win_numbers[i]; i++)
            for (int j = 0; the_numbers[j]; j++)
                if (!strcmp(the_numbers[j], win_numbers[i]))
                    match = match ? match * 2 : 1;
        sum += match;
    }

    return sum;
}

uint64_t part2(void)
{
    uint64_t sum = 0;
    int fd = open("input.txt", O_RDONLY);
    char line[LINE_SIZE];
    size_t current = 0, scratchcards[LINES];

    for (int i = 0; i < LINES; i++)
        scratchcards[i] = 1;
    while (get_next_line(fd, line) > 0) {
        char **foo = split(line,    ':');
        char **bar = split(foo[1],  '|');
        char **win_numbers = split(bar[0], ' ');
        char **the_numbers = split(bar[1], ' ');
        int  match = 0;
        for (int i = 0; win_numbers[i]; i++)
            for (int j = 0; the_numbers[j]; j++)
                if (!strcmp(the_numbers[j], win_numbers[i]))
                    match += 1;
        for (int i = 1; (current + i < LINES) && (match-- > 0); i++)
            scratchcards[current + i] += scratchcards[current];
        current++;
    }

    for (int i = 0; i < LINES; i++) { sum += scratchcards[i]; }
    return sum;
}

int main(void)
{
    //assert(part1() == 25183);
    assert(part2() == 5667240);
}
