#include "split.h"
#include "get_next_line.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define RED_LIMIT   12
#define GREEN_LIMIT 13
#define BLUE_LIMIT  14

int part1(const char *line)
{

    char game_infos[256], c[8];
    int v, game_id;

    sscanf(line, "Game %d: %[^$]", &game_id, game_infos);
    char **game = split(game_infos, ';');
    while (*game) {
        char **colors = split(*game++, ',');
        while (*colors) {
            sscanf(*colors++, "%d %s", &v, c);
            if      (!strcmp(c, "red"))   { if (v > RED_LIMIT)   return 0; }
            else if (!strcmp(c, "green")) { if (v > GREEN_LIMIT) return 0; }
            else if (!strcmp(c, "blue"))  { if (v > BLUE_LIMIT)  return 0; }
        }
    }
    return game_id;
}

int part2(const char *line)
{

    char game_infos[256], c[8];
    int r, g, b, v, game_id;

    sscanf(line, "Game %d: %[^$]", &game_id, game_infos);
    char **game = split(game_infos, ';');
    r = g = b = 0;
    while (*game) {
        char **colors = split(*game++, ',');
        while (*colors) {
            sscanf(*colors++, "%d %s", &v, c);
            if      (!strcmp(c, "red"))   { r = MAX(r, v); }
            else if (!strcmp(c, "green")) { g = MAX(g, v); }
            else if (!strcmp(c, "blue"))  { b = MAX(b, v); }
        }
    }
    return r * g * b;
}

uint32_t aoc(int (*f)(const char *))
{
    int fd = open("input.txt", O_RDONLY);
    uint32_t sum = 0;
    char line[LINE_SIZE];

    while (get_next_line(fd, line) > 0)
        sum += f(line);

    close(fd);
    return sum;
}

int main(void)
{
    //assert(aoc(part1) == 1867);
    assert(aoc(part2) == 84538);

}
