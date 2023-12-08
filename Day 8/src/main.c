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
    for(short i = 0, a = 0, b = 0, c = 0;
        a != 25 && b != 25 && c != 25; i++, sum++) {
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

struct Pair {
    short a;
    short b;
    short c;
};

uint64_t part2(void)
{
    uint64_t sum = 0;
    short nn = 0;
    int fd = open("input.txt", O_RDONLY);
    struct Pair start_nodes[512];
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
        if (c == 0) { 
            start_nodes[nn++] = (struct Pair) {
                .a = a, 
                .b = b, 
                .c = c,
            }; 
        }
    }
    while (1) {
        for (short i = 0;; i++, sum++) {
            if (!directions[i])
                i = 0;
            bool end = true;
            for (short j = 0; j < nn; j++) {
                const struct Pair current = start_nodes[j];
                if (directions[i] == 'L') {
                    start_nodes[j].a =  n[current.a][current.b][current.c].left[0] - 0x41;
                    start_nodes[j].b =  n[current.a][current.b][current.c].left[1] - 0x41;
                    start_nodes[j].c =  n[current.a][current.b][current.c].left[2] - 0x41;
                } else if (directions[i] == 'R') {
                    start_nodes[j].a = n[current.a][current.b][current.c].right[0] - 0x41;
                    start_nodes[j].b = n[current.a][current.b][current.c].right[1] - 0x41;
                    start_nodes[j].c = n[current.a][current.b][current.c].right[2] - 0x41;
                }
                if (start_nodes[j].c != 25) { end = false; } 
            }
            if (end) { return ++sum; }
        }
    }
    return sum;
}

int main(void)
{
    assert(part1() == 22199);
    assert(part2() == 0);
}
