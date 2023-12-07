#ifndef combinationsH
#define combinationsH

#define COMBINATION_SIZE 5

// A, K, Q, J, T, 9, 8, 7, 6, 5, 4, 3, or 2.
static const char SYMBOLS[] = {
	'A',
	'K',
	'Q',
	'J',
	'T',
	'9',
	'8',
	'7',
	'6',
	'5',
	'4',
	'3',
	'2',
};

static const short NBR_OF_SYMBOLS =
	sizeof(SYMBOLS) / sizeof(const char);

#endif
