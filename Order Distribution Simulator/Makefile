N_THREADS = 4
# N_THREADS = 12 # If default value in source files is preferred, then comment out this line like above

CC = gcc
CFLAGS = -g3 -O0 -std=c11 -Wall -Wextra -Wpedantic
LDLIBS = -pthread -latomic

SRC = main.c core.c
INCLUDE = core.h
TARGET = main

ifdef N_THREADS
	CFLAGS += -DN_THREADS=$(N_THREADS)
endif

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC) $(INCLUDE) Makefile
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDLIBS)

clean:
	rm -f $(TARGET)