EXEC = executavel

CC = gcc

CFLAGS = -Wall -Wextra -g

SRC = main.c fornecidas.c funcoes.c registros.c

OBJ = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

%.o: %.c registros.h
	$(CC) $(CFLAGS) -c $<

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(OBJ) $(EXEC)
