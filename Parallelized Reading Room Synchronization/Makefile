CC = gcc
CFLAGS = -pthread
TARGET = semaphores

SRC = semaphores.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)