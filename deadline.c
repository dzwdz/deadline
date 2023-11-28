#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* NIH isdigit to avoid locale bullshit */
bool
isnum(char c)
{
	return ('0' <= c) && (c <= '9');
}

/* consumes [0-9][0-9]?
 * returns 0 on success, -1 on failure */
int
parseseg(const char *s, const char **sout, int *nout, int mindigits)
{
	int n = 0;
	assert(s && sout && nout);
	assert(mindigits == 1 || mindigits == 2);
	if (!isnum(*s)) {
		return -1;
	}
	n = *s++ - '0';
	if (isnum(*s)) {
		n = n * 10 + *s++ - '0';
	} else if (mindigits == 2) {
		return -1;
	}
	*sout = s;
	*nout = n;
	return 0;
}

int
time_parse(const char *s)
{
	/* should only accept /\d\d?\(:\d\d\)/
	 * the parsing is strict to allow extensibility later
	 * blame bx, lol */
	int h = 0, m = 0;
	assert(s);
	if (parseseg(s, &s, &h, 1) < 0) {
		errx(1, "invalid hour specifier");
	}
	if (*s != '\0') {
		if (*s++ != ':') {
			errx(1, "hour and minute must be separated by ':'");
		}
		if (parseseg(s, &s, &m, 2) < 0) {
			errx(1, "invalid minute specifier");
		}
		if (*s != '\0') {
			errx(1, "junk after the minute specifier");
		}
	}

	assert(0 <= h && 0 <= m);
	if (h == 24 && m == 0) return 0; /* 24:00 is valid */
	if (24 <= h) {
		errx(1, "hour not in range");
	}
	if (60 <= m) {
		errx(1, "minute not in range");
	}
	return h * 60 + m;
}

int
hour(int n)
{
	n = n / 60;
	assert(0 <= n && n < 24);
	return n;
}

int
minute(int n)
{
	return n % 60;
}

int
main(int argc, char **argv)
{
	if (argc < 2) {
		errx(1, "no arguments");
	} else if (argc == 2) {
		int t = time_parse(argv[1]);
		fprintf(stderr, "got only one argument, assuming you're testing the hour parser...\n");
		printf("%02d:%02d\n", hour(t), minute(t));
	} else {
		int from = time_parse(argv[1]);
		int to   = time_parse(argv[2]);
		if (from == to) {
			errx(1, "\"from\" and \"to\" times must be different");
		}
	}
}
