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

struct Lcm {
    uint64_t v;
    uint64_t n;
};

uint64_t max_cycle(
        const struct Node n[26][26][26],
        struct Pair *current,
        const char *directions,
        size_t i,
        uint64_t sum
)
{

   struct Pair this = *current;
   if (directions[i] == 'L') {
       this.a =  n[current->a][current->b][current->c].left[0] - 0x41;
       this.b =  n[current->a][current->b][current->c].left[1] - 0x41;
       this.c =  n[current->a][current->b][current->c].left[2] - 0x41;
   } else if (directions[i] == 'R') {
       this.a = n[current->a][current->b][current->c].right[0] - 0x41;
       this.b = n[current->a][current->b][current->c].right[1] - 0x41;
       this.c = n[current->a][current->b][current->c].right[2] - 0x41;
   }
   if (!directions[++i])
       i = 0;
   *current = this;
   if (this.c == 25) {
       return sum + 1;
   } else { return max_cycle(n, current, directions, i, sum + 1); }
}

uint64_t part2(void)
{
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
            start_nodes[nn] = (struct Pair) {
                .a = a, 
                .b = b, 
                .c = c,
            }; 
            nn++;
        }
    }
    struct Lcm lcm[6];
    for (short i = 0; i < nn; i++) {
        struct Pair tmp = (struct Pair) {
            .a = start_nodes[i].a, 
            .b = start_nodes[i].b, 
            .c = start_nodes[i].c 
        };
        lcm[i].n = lcm[i].v = max_cycle(n, &tmp, directions, 0, 0);
    }
    while (1) {
        uint64_t ref = lcm[0].v;
        short i = 0;
        for (; i < nn; i++)
            if (lcm[i].v != ref)
                break ;
        if (i == nn) return ref;
        uint64_t min = 0;
        for (short i = 0; i < nn; i++) {
            if (lcm[i].v < lcm[min].v)
                min = i;
        }
        lcm[min].v += lcm[min].n;
    }
    return 0x1337u;
}

int main(void)
{
    assert(part1() == 22199);
    assert(part2() == 13334102464297);
}
