# INITIAL COMPILE

# Note: pkg-config is skipped since Raylib isn't system-installed/configured.
# Compile all source files and link with required libraries.

gcc main.c test.c -o game.out -I include/ -L lib/ -lraylib -lm -lX11 -lGL -lpthread -ldl

./game.out