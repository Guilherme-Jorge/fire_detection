CC = gcc
CFLAGS = -Wall -pthread

all: fire-detection

fire-detection: fire-detection.o functions.o
	$(CC) $(CFLAGS) -o fire-detection fire-detection.o functions.o

fire-detection.o: fire-detection.c
	$(CC) $(CFLAGS) -c fire-detection.c

functions.o: functions.c
	$(CC) $(CFLAGS) -c functions.c

clean:
	rm -f *.o fire-detection