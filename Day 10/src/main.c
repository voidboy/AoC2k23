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
#define WIDTH  140
#define HEIGHT WIDTH

enum Dir {
    TOP,
    BOT,
    LEFT,
    RIGHT,
};

struct Point {
    uint64_t x;
    uint64_t y;
    enum Dir d;
};

inline void
find_start(char map[HEIGHT][WIDTH], struct Point *p)
{
    for (size_t i = 0; i < HEIGHT; i++) {
        for (size_t j = 0; j < WIDTH; j++) {
            if (map[i][j] == 'S') {
                p->y = i;
                p->x = j;
                return  ;
            }
        }
    }
}

inline void
loop_entries(
        char map[HEIGHT][WIDTH],
        struct Point *start,
        struct Point adj[2]
)
{
    short i = 0;

    // NORTH ^
    if (start->y - 1 > 0) {
        switch (map[start->y - 1][start->x]) {
        case '|':
        case '7':
        case 'F':
            adj[i++] = (struct Point) { 
                .x = start->x, .y = start->y - 1, .d = TOP
            };
        }
    }
    // SOUTH v
    if (start->y + 1 < HEIGHT) {
        switch (map[start->y + 1][start->x]) {
        case '|':
        case 'L':
        case 'J':
            adj[i++] = (struct Point) {
                .x = start->x, .y = start->y + 1, .d = BOT
            };
        }
    }
    // EAST  > 
    if (start->x + 1 < WIDTH) {
        switch (map[start->y][start->x + 1]) {
        case '-':
        case 'J':
        case '7':
            adj[i++] = (struct Point){ 
                .x = start->x + 1, .y = start->y, .d = RIGHT
            };
        }
    }
    // WEST  <
    if (start->x - 1 > 0) {
        switch (map[start->y][start->x + 1]) {
        case '-':
        case 'L':
        case 'F':
            adj[i++] = (struct Point) {
                .x = start->x - 1, .y = start->y, .d = LEFT
            };
        }
    }
    assert(i == 2);
}

inline void move_point(struct Point *p, char map[HEIGHT][WIDTH])
{
    if (map[p->y][p->x] == '|')
        if (p->d == TOP)  { p->y -= 1; } else { p->y += 1; }
    else if (map[p->y][p->x] == '-')
        if (p->d == LEFT) { p->x -= 1; } else { p->x += 1; }
    else if (map[p->y][p->x] == 'L')
        if (p->d == BOT) { p->x += 1; p->d = RIGHT; } else { p->y -= 1; p->d = TOP; }
    else if (map[p->y][p->x] == 'J')
        if (p->d == BOT) { p->x -= 1; p->d = LEFT;  } else { p->y -= 1; p->d = TOP; }
    else if (map[p->y][p->x] == '7')
        if (p->d == TOP) { p->x -= 1; p->d = LEFT;  } else { p->y += 1; p->d = BOT; }
    else if (map[p->y][p->x] == 'F')
        if (p->d == TOP) { p->x += 1; p->d = RIGHT; } else { p->y += 1; p->d = BOT; }
}

uint64_t part1(void)
{
    uint64_t sum = 1;
    int fd = open("input.txt", O_RDONLY);
    char line[LINE_SIZE];
    char map[HEIGHT][WIDTH];
    struct Point start, adj[2];

    for (size_t i = 0; get_next_line(fd, line) > 0; i++) 
        strncpy(map[i], line, WIDTH);
    find_start(map, &start);
    printf("Start @ [%3lu,%3lu]\n", start.x,  start.y);
    loop_entries(map, &start, &adj);
    printf("Adj1  @ [%3lu,%3lu]\n", adj[0].x, adj[0].y);
    printf("Adj2  @ [%3lu,%3lu]\n", adj[1].x, adj[1].y);
    while (adj[0].x != adj[1].x || adj[0].y != adj[1].y) {
        move_point(&adj[0], map);
        move_point(&adj[1], map);
        sum++;
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
    printf("part. 1 is %ld\n", part1());
    //assert(part1() == 0);
    //assert(part2() == 0);
}
