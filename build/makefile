# Directories
SRC_DIR := ../src
BIN_DIR := ../bin

# Files
TESTS := $(SRC_DIR)/tests.c
SOLVE := $(SRC_DIR)/solve.c
FAIL  := $(SRC_DIR)/fail.c

# Executables
SOLVE_EXE := $(BIN_DIR)/solve
FAIL_EXE  := $(BIN_DIR)/fail

# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11 -O2
LDFLAGS := -lcriterion -lm

# Rules
.PHONY: all clean

all: $(SOLVE_EXE) $(FAIL_EXE)

$(SOLVE_EXE): $(SOLVE) $(TESTS)
	$(CC) $(CFLAGS) -o $@ $(SOLVE) $(TESTS) $(LDFLAGS)

$(FAIL_EXE): $(FAIL) $(TESTS)
	$(CC) $(CFLAGS) -o $@ $(FAIL) $(TESTS) $(LDFLAGS)

clean:
	rm -f $(BIN_DIR)/*
