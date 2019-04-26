CC ?= gcc
CFLAGS := ${CFLAGS} -O2 -Wextra -Wall -Wfloat-equal -Wundef -Wpointer-arith -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -Winit-self -std=c99
TARGETS := main.o

.PHONY: all
all: build run clean

.PHONY: debug
debug: pre-debug build debug-run clean

.PHONY: debug-run
debug-run:
	gdb main.o

.PHONY: pre-debug
pre-debug:
	$(eval CFLAGS = ${CFLAGS} -g)

.PHONY: run
run:
	./main.o

.PHONY: build
build: $(TARGETS)

%.o: %.c
	$(CC) $< -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o
