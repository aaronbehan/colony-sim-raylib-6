# This updated Makefile should allow me to simply add new .c files without having to copy paste the old .c to .o syntax every time

# Compiler and flags
CC = gcc
CFLAGS = -g -I src/include/

# Source and objects
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

# Output
OUT = main.out

# Linker flags for Linux + Raylib
LDFLAGS = -L lib/ -lraylib -lm -lX11 -lGL -lpthread -ldl

# Build
all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(OUT)