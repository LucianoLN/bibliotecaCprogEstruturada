CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
HDRS = $(wildcard include/*.h)
TARGET = biblioteca
TEST_BIN = tests/run_tests

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

src/%.o: src/%.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

# Testes com assert: todos os modulos menos o main.c do menu.
test: $(filter-out src/main.o,$(OBJS)) tests/test_books.c
	$(CC) $(CFLAGS) -o $(TEST_BIN) tests/test_books.c $(filter-out src/main.o,$(OBJS))
	./$(TEST_BIN)

clean:
	-rm -f $(OBJS) $(TARGET) $(TARGET).exe $(TEST_BIN) $(TEST_BIN).exe
