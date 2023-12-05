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

struct Foo {
    uint64_t number;
    bool     dirty;
    bool     stopy;
};

uint64_t part1(void)
{
    uint64_t min = UINT64_MAX;
    int fd = open("input.txt", O_RDONLY);
    char line[LINE_SIZE];
    struct Foo *seeds_number = malloc(sizeof(struct Foo) * 64);

    get_next_line(fd, line);
    char **seeds = split(split(line, ':')[1], ' ');
    size_t index = 0;
    for (; seeds[index]; index++) {
        seeds_number[index] = (struct Foo) {
            .number = atoll(seeds[index]),
            .dirty  = false,
            .stopy  = false,
        };
    }
    seeds_number[index] = (struct Foo) {
        .stopy = true
    };
    while (get_next_line(fd, line) != -1) {
        uint64_t d = 0, s = 0, r = 0;
        if (sscanf(line, "%lu %lu %lu", &d, &s, &r) == 3) {
            do {
                for (int i = 0; !seeds_number[i].stopy; i++) {
                    // range are not inclusive 
                    if (  !seeds_number[i].dirty 
                        && seeds_number[i].number >= s
                        && seeds_number[i].number < (s + r)) {
                        seeds_number[i].number =
                            d + (seeds_number[i].number - s);
                        seeds_number[i].dirty = true;
                    }
                }
                get_next_line(fd, line);
            } 
            while (sscanf(line, "%lu %lu %lu", &d, &s, &r) == 3);
            for (int i = 0; !seeds_number[i].stopy; i++)
                seeds_number[i].dirty = false;
        }
    }
    for (index = 0; !seeds_number[index].stopy; index++)
        min = MIN(min, seeds_number[index].number);
    return min;
}


struct Bar {
    uint64_t number;
    uint64_t range;
    bool     stopy;
};

struct Rng {
    uint64_t new;
    uint64_t start;
    uint64_t range;
};

static inline bool dejavu(uint64_t this, struct Bar *seeds_number, int index)
{
    while (--index >= 0) {
        if (   seeds_number[index].number <= this 
            && seeds_number[index].number + seeds_number[index].range > this)
                return true;
    }
    return false; 
}

uint64_t part2(void)
{
    uint64_t min = UINT64_MAX;
    int fd = open("input.txt", O_RDONLY);
    char line[LINE_SIZE];
    struct Bar *seeds_number = malloc(sizeof(struct Bar) * 64);
    get_next_line(fd, line);
    char **seeds = split(split(line, ':')[1], ' ');
    int index = 0;
    struct Rng ranges[7][64];

    for (int i = 0; seeds[index]; i++, index += 2) {
        seeds_number[i] = (struct Bar) {
            .number = atoll(seeds[index]),
            .range  = atoll(seeds[index + 1]),
            .stopy  = false,
        };
        seeds_number[i + 1] = (struct Bar) {
            .stopy = true
        };
    }
    for (int i = 0, k = 0; get_next_line(fd, line) != -1; i++) {
        uint64_t j = 0, d = 0, s = 0, r = 0;
        if (sscanf(line, "%lu %lu %lu", &d, &s, &r) == 3) {
            do {
                ranges[k][j++] = (struct Rng) {
                    .new = d,
                    .start = s,
                    .range = r,
                };
                get_next_line(fd, line);
            } 
            while (sscanf(line, "%lu %lu %lu", &d, &s, &r) == 3);
            ranges[k++][j].range = 0;
        }
    }
    close(fd);

    for (int i = 0; !seeds_number[i].stopy; i++) {
        for (size_t j = 0; j < seeds_number[i].range; j++) {
            uint64_t imdone = seeds_number[i].number + j;
            if (dejavu(imdone, seeds_number, i)) continue ;
            for (int k = 0; k < 7; k++ ) {
                for (int l = 0; ranges[k][l].range != 0; l++) {
                    if (   imdone >= ranges[k][l].start
                        && imdone <  ranges[k][l].start + ranges[k][l].range) {
                        imdone = ranges[k][l].new + (imdone - ranges[k][l].start);
                        break ;
                    }
                }
            }
            min = MIN(imdone, min);
        }
    }
    return min;
}

int main(void)
{
    assert(part1() == 836040384);
    assert(part2() == 10834440); // 7min ughhhhhhhh
}
