CFLAGS = -Wall -Wextra -O2 -g

.PHONY: all clean test
all: deadline
clean:
	rm -f deadline
test: deadline
	./test.sh
