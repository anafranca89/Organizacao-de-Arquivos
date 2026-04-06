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
