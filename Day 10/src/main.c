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
#define WIDTH  140
#define HEIGHT 140

enum Dir {
    TOP   = 1,
    BOT   = 2,
    LEFT  = 4,
    RIGHT = 8,
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

inline enum Dir
loop_entries(
        char map[HEIGHT][WIDTH],
        struct Point *start,
        struct Point adj[2]
)
{
    short i = 0;
    enum Dir d = 0;

    // NORTH ^
    if (start->y - 1 > 0) {
        switch (map[start->y - 1][start->x]) {
        case '|':
        case '7':
        case 'F':
            adj[i++] = (struct Point) { 
                .x = start->x, .y = start->y - 1, .d = TOP
            };
            d |= TOP;
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
            d |= BOT;
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
            d |= RIGHT;
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
            d |= LEFT;
        }
    }
    assert(i == 2); return d;
}

inline void move_point(struct Point *p, char map[HEIGHT][WIDTH])
{
    if (map[p->y][p->x] == '|') {
        if (p->d == TOP)  { p->y -= 1; } else { p->y += 1; }
    } else if (map[p->y][p->x] == '-') {
        if (p->d == LEFT) { p->x -= 1; } else { p->x += 1; }
    } else if (map[p->y][p->x] == 'L') {
        if (p->d == BOT) { p->x += 1; p->d = RIGHT; } else { p->y -= 1; p->d = TOP; }
    } else if (map[p->y][p->x] == 'J') {
        if (p->d == BOT) { p->x -= 1; p->d = LEFT;  } else { p->y -= 1; p->d = TOP; }
    } else if (map[p->y][p->x] == '7') {
        if (p->d == TOP) { p->x -= 1; p->d = LEFT;  } else { p->y += 1; p->d = BOT; }
    } else if (map[p->y][p->x] == 'F') {
        if (p->d == TOP) { p->x += 1; p->d = RIGHT; } else { p->y += 1; p->d = BOT; }
    }
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
    loop_entries(map, &start, &adj);
    while (adj[0].x != adj[1].x || adj[0].y != adj[1].y) {
        move_point(&adj[0], map);
        move_point(&adj[1], map);
        sum++;
    }
    return sum;
}

bool is_loop(struct Point *loop, uint64_t loop_points, struct Point *p)
{
    for (size_t i = 0; i < loop_points; i++)
        if (loop[i].x == p->x && loop[i].y == p->y)
            return true;
    return false;
}

size_t count_cross_loop(
        struct Point *loop,
        uint64_t loop_points,
        char map[HEIGHT][WIDTH],
        struct Point *p
)
{
    size_t counter = 0;

    for (int i = p->x - 1; i >= 0;) {
        struct Point current = {.x = i, .y = p->y};
        if (is_loop(loop, loop_points, &current)) {
            switch (map[current.y][current.x]) {
            case '|': { counter++; i--; }
            break ;
            case '7': {
                while (--i && map[current.y][i] == '-')
                    ;
                if (map[current.y][i] == 'L') { counter++; }
            }
            break ;
            case 'J': {
                while (--i && map[current.y][i] == '-')
                    ;
                if (map[current.y][i] == 'F') { counter++; }
            }
            break ;
            default: i--;
            }
        } else {
            i--;
        }
    }

    return counter;
}

void patch_start(char map[HEIGHT][WIDTH], struct Point *s, enum Dir d)
{
    if      (d == (TOP  |   BOT)) { map[s->y][s->x] = '|'; } 
    else if (d == (TOP  |  LEFT)) { map[s->y][s->x] = 'J'; } 
    else if (d == (TOP  | RIGHT)) { map[s->y][s->x] = 'L'; } 
    else if (d == (LEFT | RIGHT)) { map[s->y][s->x] = '-'; } 
    else if (d == (BOT  | RIGHT)) { map[s->y][s->x] = 'F'; } 
    else if (d == (BOT  |  LEFT)) { map[s->y][s->x] = '7'; } 
}

uint64_t part2(void)
{
    uint64_t loop_points = 0, sum = 0;
    int fd = open("input.txt", O_RDONLY);
    char line[LINE_SIZE];
    char map[HEIGHT][WIDTH];
    struct Point start, adj[2], loop[HEIGHT * WIDTH];

    for (size_t i = 0; get_next_line(fd, line) > 0; i++) 
        strncpy(map[i], line, WIDTH);
    find_start(map, &start);
    enum Dir d = loop_entries(map, &start, &adj);
    patch_start(map, &start, d);
    loop[loop_points++] = start;
    do {
        loop[loop_points++] = adj[0];
        move_point(&adj[0], map);
    } while (adj[0].x != start.x || adj[0].y != start.y);
    for (size_t i = 0; i < HEIGHT; i++) {
        for (size_t j = 0; j < WIDTH; j++) {
            struct Point current = { .x = j, .y = i };
            if (!is_loop(loop, loop_points, &current)) {
                size_t n = count_cross_loop(
                        loop, loop_points, map, &current
                );
                if (n && n % 2 != 0)
                    sum++;
            }
        }
    }
    return sum;
}

int main(void)
{
    assert(part1() == 6931);
    assert(part2() == 357);
}
