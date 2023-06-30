CC = gcc
CFLAGS = -Wall -Wextra

TARGET = client

.PHONY: all clean

all: $(TARGET)

$(TARGET): cliente.c
	$(CC) $(CFLAGS) -o $(TARGET) cliente.c

clean:
	rm -f $(TARGET)

