CC = gcc
CFLAGS = -Wall -Wextra

TARGET = server

$(TARGET): servidor.o
	$(CC) $(CFLAGS) -o $(TARGET) servidor.o -lsystemd


servidor.o: servidor.c
	$(CC) $(CFLAGS) -c servidor.c 

clean:
	rm -f $(TARGET) *.o

