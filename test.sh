#!/bin/sh
# TODO test in C

valid() {
	./deadline $1 >/dev/null 2>/dev/null || echo wrongly rejected: $1
	PARSED="$(./deadline $1 2>/dev/null)"
	if [ "$2" != "$PARSED" ]; then
		echo wrong result: $1: expected $2, got $PARSED
	fi
}

invalid() {
	./deadline $1 >/dev/null 2>/dev/null && echo wrongly accepted: $1
}

echo testing hour parsing...

valid 0       00:00
valid 0:00    00:00
valid 00      00:00
valid 00:00   00:00
valid 12      12:00
valid 12:00   12:00
valid 24      00:00
valid 24:00   00:00
valid 4:20    04:20
valid 04:20   04:20
valid 13:37   13:37
valid 21:37   21:37

invalid 0:0
invalid 4:2
invalid 04:2

invalid 24:01
invalid 25:00
invalid 99
invalid -1
invalid -0
invalid +0

invalid 000
invalid 010
invalid 000:00
invalid 00:000
invalid 00.00
invalid 00-00
invalid a00
invalid 00a
invalid 00:00:00

valid 13:59   13:59
invalid 13:60

echo done
