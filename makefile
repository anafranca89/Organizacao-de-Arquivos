<<<<<<< HEAD
EXEC = executavel

CC = gcc

CFLAGS = -Wall -Wextra -g

SRC = main.c fornecidas.c funcoes.c registros.c
OBJ = $(SRC:.c=.o)

HEADERS = registros.h ponto.h fornecidas.h funcoes.h

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(OBJ) $(EXEC)
=======
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
>>>>>>> ad50ee39c61771b70c2701147fcd4298c8c1bb9b
