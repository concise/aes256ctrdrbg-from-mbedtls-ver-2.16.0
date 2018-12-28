CC = gcc

CFLAGS = -std=c99
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -Wall -Wextra -Wpedantic -Winline
CFLAGS += -I./include/
#CFLAGS += -fsanitize=address

SRC_FILES = $(wildcard *.c library/*.c)
DEP_FILES = $(wildcard *.c library/*.c *.h include/mbedtls/*.h Makefile)
EXE = a.out

$(EXE): $(DEP_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(EXE)

.PHONY: clean
clean:
	$(RM) $(EXE)
