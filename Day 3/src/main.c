#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include "get_next_line.h"
#include "split.h"

uint32_t part1(const char *line)
{
    return 0;
}

uint32_t part2(const char *line)
{
    return 0;
}

static bool _is_digit(const char c)
{
    if (c >= '0' && c <= '9')
        return true;
    else return false;
}

static bool _is_symbol(const char c)
{
    if (c == '.') return false;
    switch (c) {
        case 0x21 ... 0x2f:
        case 0x3a ... 0x40:
        case 0x5b ... 0x60:
        case 0x7b ... 0x7e:
            return true;
        default: return false;
    }
}

// left - top - bot - right
static bool is_adjacent_to_symbol(const char **lines, int i, int j)
{
    // left
    if      (j > 0          && _is_symbol(lines[i][j - 1]))             return true;
    // top - left
    else if (j > 0 && i > 0 && _is_symbol(lines[i - 1][j - 1]))         return true;
    // top - right
    else if (j > 0 && i > 0 && _is_symbol(lines[i - 1][j + 1]))         return true;
    // top
    else if (i > 0          && _is_symbol(lines[i - 1][j]))             return true;
    // bot - left
    else if (lines[i + 1]  && j > 0 && _is_symbol(lines[i + 1][j - 1])) return true;
    // bot - right
    else if (lines[i + 1]   && _is_symbol(lines[i + 1][j + 1]))         return true;
    // bot
    else if (lines[i + 1]   && _is_symbol(lines[i + 1][j]))             return true;
    // right
    else if (_is_symbol(lines[i][j + 1]))                               return true;
    else                                                                return false;
}

uint32_t aoc(uint32_t (*f)(const char *))
{
    int fd = open("input.txt", O_RDONLY);
    struct stat file_stat;
    uint32_t sum = 0;
    char *file_content;

    fstat(fd, &file_stat);
    file_content = malloc(file_stat.st_size + 1);
    read(fd, file_content, file_stat.st_size);
    file_content[file_stat.st_size] = '\0';
    const char **lines = split(file_content, '\n');
    for (int i = 0; lines[i]; i++) {
        for (int j = 0; lines[i][j]; j++) {
            if (_is_digit(lines[i][j])) {
                int start  = j;
                bool found = false;
                // consume the number
                while (_is_digit(lines[i][j])) {
                    if (!found && is_adjacent_to_symbol(lines, i, j)) {
                        found = true;
                        sum += atoi(&lines[i][start]);
                    }
                    j++;
                }
            }
        }
    }

    return sum;
}

int main(void)
{
     printf("%u\n", aoc(part1));
    //assert(aoc(part1) == 0);
    //assert(aoc(part2) == 0);
}
