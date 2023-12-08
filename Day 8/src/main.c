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

struct Node {
    char  left[3];
    char right[3];
};

uint64_t part1(void)
{
    uint64_t sum = 0;
    int fd = open("input.txt", O_RDONLY);
    char line[LINE_SIZE];
    char directions[512];
    struct Node n[26][26][26];

    get_next_line(fd, line); // directions
    strcpy(directions, line);
    get_next_line(fd, line); // empty
    for (short a, b, c; get_next_line(fd, line) > 0;) {
        a = (uint8_t)line[0] - 0x41;
        b = (uint8_t)line[1] - 0x41;
        c = (uint8_t)line[2] - 0x41;
        strncpy(n[a][b][c].left,  &( line[7]), 3);
        strncpy(n[a][b][c].right, &(line[12]), 3);
    }
    short i, a, b, c;
    i = a = b = c = 0;
    for(; a != 25 && b != 25 && c != 25; i++, sum++) {
        if (!directions[i])
            i = 0;
        const struct Node *current = &(n[a][b][c]);
        if (directions[i] == 'L') {
            a = (uint8_t)(current->left[0])  - 0x41;
            b = (uint8_t)(current->left[1])  - 0x41;
            c = (uint8_t)(current->left[2])  - 0x41;
        } else if (directions[i] == 'R') {
            a = (uint8_t)(current->right[0]) - 0x41;
            b = (uint8_t)(current->right[1]) - 0x41;
            c = (uint8_t)(current->right[2]) - 0x41;
        }
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
    assert(part1() == 22199);
    assert(part2() == 0);
}
