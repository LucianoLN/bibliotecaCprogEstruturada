CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
SRCS = src/book.c src/clic.c src/fine.c src/main.c src/persist.c src/report.c src/search.c src/sort.c src/storage.c src/util.c
OBJS = $(SRCS:.c=.o)
HDRS = $(wildcard include/*.h)
TARGET = biblioteca
TEST_BIN = tests/run_tests
TEST_SRCS = $(filter-out src/clic.c src/main.c,$(SRCS))

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

src/%.o: src/%.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

# Testes com assert: todos os modulos menos o main.c do menu.
test: tests/test_books.c $(TEST_SRCS)
	$(CC) $(CFLAGS) -o $(TEST_BIN) tests/test_books.c $(TEST_SRCS)
	./$(TEST_BIN)

clean:
	-rm -f $(OBJS) $(TARGET) $(TARGET).exe $(TEST_BIN) $(TEST_BIN).exe
