CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Iinclude
SRC = src/main.c src/book.c src/user.c src/loan.c src/storage.c
OBJ = $(SRC:.c=.o)
TARGET = biblioteca

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
