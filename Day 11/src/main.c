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

struct Point {
    uint64_t x;
    uint64_t y;
};

bool is_empty_row(char line[512])
{
    while (*line) {
        if (*line != '.')
            return false;
        line++;
    }
    return true;
}

bool is_empty_col(char foo[512][512], uint64_t index)
{
    for (size_t i = 0; foo[i][index]; i++)
        if (foo[i][index] != '.')
            return false;
    return true;
}

void expansion(
        char     univers[512][512],
        char   multivers[512][512]
)
{
    // col 
    for (size_t i = 0, l = 0; univers[0][i]; i++) {
        if (is_empty_col(univers, i)) {
            for (size_t k = 0; univers[k][0]; k++) {
                multivers[k][l]     = '.';
                multivers[k][l + 1] = '.';
            }
            l++;
            l++;
        } else {
            for (size_t k = 0; univers[k][0]; k++)
                multivers[k][l] = univers[k][i];
            l++;
        }
    }
    for (size_t i = 0; multivers[0][i]; i++)
        strcpy(univers[i], multivers[i]);
    // row
    for (size_t i = 0, k = 0; univers[0][i]; i++) {
        if (is_empty_row(univers[i])) {
            strcpy(multivers[k],     univers[i]);
            strcpy(multivers[k + 1], univers[i]);
            k++;
            k++;
        } else {
            strcpy(multivers[k], univers[i]);
            k++;
        }
    }
}

void find_galaxies(
        char   multivers[512][512],
        struct Point galaxies[512],
        uint64_t  *nbr_of_galaxies
)
{
    for (size_t i = 0; multivers[0][i]; i++)
        for (size_t j = 0; multivers[i][j]; j++)
            if (multivers[i][j] == '#') 
                galaxies[(*nbr_of_galaxies)++] = (struct Point){.x = j, .y = i};
}

size_t shortest_path(struct Point *s, struct Point *d)
{
    return MAX(s->x, d->x) - MIN(s->x, d->x) + MAX(s->y, d->y) - MIN(s->y, d->y);
}

size_t shortest_path_but_1M(
        struct Point *s,
        struct Point *d,
        char universe[512][512]
)
{
    uint64_t dist   = 0;
    uint64_t to_x   = MAX(s->x, d->x);
    uint64_t from_x = MIN(s->x, d->x);
    uint64_t to_y   = MAX(s->y, d->y);
    uint64_t from_y = MIN(s->y, d->y);

    while (from_x != to_x) {
        if (is_empty_col(universe, from_x))
            dist += 1e6;
        else 
            dist += 1;
        from_x++;
    }
    while (from_y != to_y) {
        if (is_empty_row(universe[from_y]))
            dist += 1e6;
        else 
            dist += 1;
        from_y++;
    }
    return dist;
}

uint64_t part1(void)
{
    uint64_t nbr_of_galaxies = 0, sum = 0;
    int fd = open("input.txt", O_RDONLY);
    char line[LINE_SIZE];
    char   univers[512][512] = {0};
    char multivers[512][512] = {0};
    struct Point galaxies[512];

   for (size_t i = 0; get_next_line(fd, line) > 0; i++)
       strcpy(univers[i], line);
   expansion(univers, multivers);
   find_galaxies(multivers, galaxies, &nbr_of_galaxies);
   for (size_t i = 0; i < nbr_of_galaxies; i++)
       for (size_t j = i + 1; j < nbr_of_galaxies; j++)
           sum += shortest_path(&galaxies[i], &galaxies[j]);
   return sum;
}

uint64_t part2(void)
{
    uint64_t nbr_of_galaxies = 0, sum = 0;
    int fd = open("input.txt", O_RDONLY);
    char line[LINE_SIZE];
    char   univers[512][512] = {0};
    struct Point galaxies[512];

   for (size_t i = 0; get_next_line(fd, line) > 0; i++)
       strcpy(univers[i], line);
   find_galaxies(univers, galaxies, &nbr_of_galaxies);
   for (size_t i = 0; i < nbr_of_galaxies; i++)
       for (size_t j = i + 1; j < nbr_of_galaxies; j++)
           sum += shortest_path_but_1M(&galaxies[i], &galaxies[j], univers);
   return sum;
}

int main(void)
{
    assert(part1() == 9639160);
    assert(part2() == 752936133304);
}
