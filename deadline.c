#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Time Time;
struct Time {
	int h, m;
};

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

Time
time_parse(const char *s)
{
	/* should only accept /\d\d?\(:\d\d\)/
	 * the parsing is strict to allow extensibility later
	 * blame bx, lol */
	Time t = {0, 0};
	assert(s);
	if (parseseg(s, &s, &t.h, 1) < 0) {
		errx(1, "invalid hour specifier");
	}
	if (*s != '\0') {
		if (*s++ != ':') {
			errx(1, "hour and minute must be separated by ':'");
		}
		if (parseseg(s, &s, &t.m, 2) < 0) {
			errx(1, "invalid minute specifier");
		}
		if (*s != '\0') {
			errx(1, "junk after the minute specifier");
		}
	}

	assert(0 <= t.h && 0 <= t.m);
	if (t.h == 24 && t.m == 0) return t; /* 24:00 is valid */
	if (24 <= t.h) {
		errx(1, "hour not in range");
	}
	if (60 <= t.m) {
		errx(1, "minute not in range");
	}
	return t;
}

int
main(int argc, char **argv)
{
	if (argc == 2) {
		Time t = time_parse(argv[1]);
		fprintf(stderr, "got only one argument, assuming you're testing the hour parser...\n");
		printf("%02d:%02d\n", t.h, t.m);
	} else {
		errx(1, "invalid usage");
	}
}
