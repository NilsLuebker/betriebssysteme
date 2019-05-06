CC ?= gcc
CFLAGS := ${CFLAGS} -O2 -pedantic -Wextra -Wall -Wfloat-equal -Wundef -Wpointer-arith -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -Winit-self -std=gnu17
OBJS := buildins.o argparse.o jobctl.o
BIN := shell
DEFS :=
LIBS :=

.PHONY: all
all: build run

.PHONY: debug
debug: pre-debug build debug-run

.PHONY: debug-run
debug-run:
	gdb $(BIN)

.PHONY: pre-debug
pre-debug:
	$(eval CFLAGS = -g -Wall -Wextra -pedantic -std=gnu17)

.PHONY: run
run:
	./$(BIN)

.PHONY: build
build: $(BIN)

$(BIN): $(OBJS) $(BIN).c
	@echo "Building: $(BIN)"
	$(CC) $(CFLAGS) $(DEFS) $(LIBS) $(OBJS) $(BIN).c -o $(BIN)

%.o: %.c %.h
	@echo "Building: $@"
	$(CC) -c $(CFLAGS) $(DEFS) $< -o $@

.PHONY: clean
clean:
	rm -f *.o $(BIN)
