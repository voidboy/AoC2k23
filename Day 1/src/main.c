#include "get_next_line.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <string.h>

static inline bool _is_digit(char c)
{
    if (((c & 0xf0) == 0x30) && ((c & 0x0f) <= 0x09))
        return true;
    else 
        return false;
}

/*
    Poor performance

static inline bool _is_digit(char c)
{
    if ((c & 0xf0) == 0x30) {
        if (c & 0x08 && (c & 0x4 || c & 0x2))
            return false;
        return true;
    } else return false;
}
*/

// first part : 54159
static uint64_t part1(void)
{
    char *line;
    int fd = open("input.txt", O_RDONLY);
    uint8_t line_length;
    uint32_t a, b;
    uint64_t sum = 0;

    while ((line_length = get_next_line(fd, &line)) > 0) {
        for (int i = 0; line[i]; i++) {
            if (_is_digit(line[i])) {
                a = (line[i] - '0') * 10;
                break ;
            }
        }
        for (int i = line_length - 1; i >= 0; i--) {
            if (_is_digit(line[i])) {
                b = line[i] - '0';
                break ;
            }
        }
        sum += a + b;
    }

    close(fd);
    return sum;
}

static inline int literal_to_digit(const char *str)
{
    if      (!strncmp("one", str, 3))   { return 1; } 
    else if (!strncmp("two", str, 3))   { return 2; }
    else if (!strncmp("three", str, 5)) { return 3; }
    else if (!strncmp("four", str, 4))  { return 4; }
    else if (!strncmp("five", str, 4))  { return 5; }
    else if (!strncmp("six", str, 3))   { return 6; }
    else if (!strncmp("seven", str, 5)) { return 7; }
    else if (!strncmp("eight", str, 5)) { return 8; }
    else if (!strncmp("nine", str, 4))  { return 9; }
    else { return 0; }
}

// second part : 53866
uint64_t part2(void)
{
    char *line;
    int fd = open("input.txt", O_RDONLY);
    uint8_t line_length;
    uint32_t a, b;
    uint64_t sum = 0;

    while ((line_length = get_next_line(fd, &line)) > 0) {
        for (int i = 0; line[i]; i++) {
            if (_is_digit(line[i])) {
                a = line[i] - '0';
                break ;
            } else if ((a = literal_to_digit(&line[i]))) {
                break ;
            }
        }
        for (int i = line_length - 1; i >= 0; i--) {
            if (_is_digit(line[i])) {
                b = line[i] - '0';
                break ;
            } else if ((b = literal_to_digit(&line[i]))) {
                break ;
            }
        }
        sum += a * 10 + b;
    }

    close(fd);
    return sum;
}

int main(void)
{
    assert(part1() == 54159);
    //assert(part2() == 53866);

    return 0;
}
