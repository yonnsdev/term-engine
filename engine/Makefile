# compiler
CC = clang
CFLAGS = -std=c11 -Wall

# src, obj, build, out
SRC = $(shell find . -name "*.c")
OBJ = $(SRC:.c=.o)

# linker
#LDFLAGS = -lncurses
LDFLAGS = 

.PHONY: clean
 
%.o: %.c engine.h
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

build: $(OBJ)
	@echo $(filter %.o, $^)

clean:
	rm -rf $(OBJ)