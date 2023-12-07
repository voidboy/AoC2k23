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
#define CARDS 13
#define HAND_SIZE 5
#define LINES 1000

enum Type {
    FIVE_KIND  = 42,
    FOUR_KIND  = 41,
    FULL_HOUSE = 40,
    THREE_KIND = 39,
    TWO_PAIR   = 38,
    ONE_PAIR   = 37,
    HIGH_CARD  = 36,
};

struct Game {
    char hand[HAND_SIZE + 1]; // nullbyte
    uint64_t bid;
    enum Type t;

};

static uint8_t label_to_index(const char label)
{
    // A, K, Q, J, T, 9, 8, 7, 6, 5, 4, 3, or 2
    switch (label) {
        case 'A': return 12;
        case 'K': return 11;
        case 'Q': return 10;
        case 'J': return  9;
        case 'T': return  8;
        case '9': return  7;
        case '8': return  6;
        case '7': return  5;
        case '6': return  4;
        case '5': return  3;
        case '4': return  2;
        case '3': return  1;
        case '2': return  0;
    }
    return 0; // compiler warning
}

static uint8_t P2_label_to_index(const char label)
{
    // A, K, Q, J, T, 9, 8, 7, 6, 5, 4, 3, 2, J
    switch (label) {
        case 'A': return 12;
        case 'K': return 11;
        case 'Q': return 10;
        case 'T': return  9;
        case '9': return  8;
        case '8': return  7;
        case '7': return  6;
        case '6': return  5;
        case '5': return  4;
        case '4': return  3;
        case '3': return  2;
        case '2': return  1;
        case 'J': return  0;
    }
    return 0; // compiler warning
}

static enum Type which_type(const char *hand)
{
    short pairs = 0, max = 0, occur[CARDS] = {0};
    for (size_t i = 0; i < HAND_SIZE; i++) {
        const uint8_t index = label_to_index(hand[i]);
        occur[index] += 1;
        max = MAX(max, occur[index]);
        if (occur[index] && ((occur[index] % 2) == 0))
            pairs++;
    }
    //printf("hand is %s, max is %d, pairs is %d\n", hand, max, pairs);
    switch (max) {
        case 5: return FIVE_KIND;
        case 4: return FOUR_KIND;
        case 3: return (pairs == 2 ? FULL_HOUSE : THREE_KIND);
        case 2: return (pairs == 2 ? TWO_PAIR   :   ONE_PAIR);
        case 1: return HIGH_CARD;
    }
    return HIGH_CARD; // compiler warning
}

static enum Type P2_which_type(const char *hand)
{
    short jokers = 0, pairs = 0, max = 0, occur[CARDS] = {0};
    for (size_t i = 0; i < HAND_SIZE; i++) {
        if (hand[i] == 'J') {
            jokers++;
        } else {
            const uint8_t index = P2_label_to_index(hand[i]);
            occur[index] += 1;
            max = MAX(max, occur[index]);
            if (occur[index] && ((occur[index] % 2) == 0))
                pairs++;
        }
    }
    //printf("hand is %s, max is %d, pairs is %d\n", hand, max, pairs);
    switch (max + jokers) {
        case 5: return FIVE_KIND;
        case 4: return FOUR_KIND;
        case 3: return (pairs == 2 ? FULL_HOUSE : THREE_KIND);
        case 2: return (pairs == 2 ? TWO_PAIR   :   ONE_PAIR);
        case 1: return HIGH_CARD;
    }
    return HIGH_CARD; // compiler warning
}

static inline void swap(struct Game *a, struct Game *b)
{
    struct Game tmp; //deep copy with array

    tmp = *a;
    *a = *b;
    *b = tmp;
}

static void sort_hands(struct Game *g)
{
    for (size_t i = LINES - 1; i > 0; i--) {
        for (size_t j = 0; j < i; j++) {
            if (g[j].t > g[j + 1].t) {
                swap(&g[j], &g[j + 1]);
            } else if (g[j].t == g[j + 1].t) {
                for (short k = 0; g[j].hand[k]; k++) {
                    const short s1 = label_to_index(g[j + 0].hand[k]);
                    const short s2 = label_to_index(g[j + 1].hand[k]);
                    if (s1 < s2) { break ; }
                    if (s1 > s2) { swap(&g[j], &g[j + 1]); break ; }
                }
            }
        }
    }
}

static void P2_sort_hands(struct Game *g)
{
    for (size_t i = LINES - 1; i > 0; i--) {
        for (size_t j = 0; j < i; j++) {
            if (g[j].t > g[j + 1].t) {
                swap(&g[j], &g[j + 1]);
            } else if (g[j].t == g[j + 1].t) {
                for (short k = 0; g[j].hand[k]; k++) {
                    const short s1 = P2_label_to_index(g[j + 0].hand[k]);
                    const short s2 = P2_label_to_index(g[j + 1].hand[k]);
                    if (s1 < s2) { break ; }
                    if (s1 > s2) { swap(&g[j], &g[j + 1]); break ; }
                }
            }
        }
    }
}

uint64_t part1(void)
{
    uint64_t sum = 0;
    int fd = open("input.txt", O_RDONLY);
    char line[LINE_SIZE];
    struct Game g[LINES];

    for (size_t i = 0; get_next_line(fd, line) > 0; i++) {
        sscanf(line, "%s %lu", g[i].hand, &(g[i].bid));
        g[i].t = which_type(g[i].hand);
    }
    sort_hands(g);
    for (size_t i = 0; i < LINES; i++) 
        sum += g[i].bid * (i + 1);
    close(fd);

    return sum;
}

uint64_t part2(void)
{
    uint64_t sum = 0;
    int fd = open("input.txt", O_RDONLY);
    char line[LINE_SIZE];
    struct Game g[LINES];

    for (size_t i = 0; get_next_line(fd, line) > 0; i++) {
        sscanf(line, "%s %lu", g[i].hand, &(g[i].bid));
        g[i].t = P2_which_type(g[i].hand);
    }
    P2_sort_hands(g);
    for (size_t i = 0; i < LINES; i++) 
        sum += g[i].bid * (i + 1);
    close(fd);

    return sum;
}


int main(void)
{
    assert(part1() == 251545216); 
    assert(part2() == 250384185);
}
