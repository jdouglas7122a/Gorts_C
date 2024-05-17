CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = interpreter
SRC = main.c lexer.c parser.c interpreter.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)
