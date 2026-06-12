CC = gcc
CFLAGS = -Wall -g
TARGET = programaTrab

all:
	$(CC) $(CFLAGS) -o $(TARGET) *.c

run:
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)



.PHONY: all run clean