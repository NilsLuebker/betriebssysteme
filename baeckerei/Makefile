CC ?= gcc
WFLAGS := -Wpedantic -Wextra -Wall -Wformat=2 -Wformat-signedness -Winit-self
DBGFLAGS := -g
CFLAGS := ${CFLAGS} -O2 -std=gnu17
OBJS := buildins.o argparse.o jobctl.o
BIN := shell
DEFS :=
LIBS := -lpthread

.PHONY: all
all: build run

.PHONY: debug
debug: pre-debug build debug-run

.PHONY: debug-run
debug-run:
	gdb $(BIN)

.PHONY: pre-debug
pre-debug:
	$(eval CFLAGS = -g -std=gnu17)

.PHONY: run
run:
	./$(BIN)

.PHONY: build
build: $(BIN)

$(BIN): $(OBJS) $(BIN).c
	@echo "Building: $(BIN)"
	$(CC) $(CFLAGS) $(WFLAGS) $(DEFS) $(LIBS) $(OBJS) $(BIN).c -o $(BIN)

%.o: %.c %.h
	@echo "Building: $@"
	$(CC) -c $(CFLAGS) $(WFLAGS) $(DEFS) $< -o $@

.PHONY: clean
clean:
	rm -f *.o $(BIN)
