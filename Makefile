EXE_NAME=game.exe
INCLUDE=include
SRCS=$(wildcard src/*.cpp)
CC=g++
FLAGS = -Wall -Werror
LFLAGS = -fsanitize=address -fsanitize=leak

all: $(SRCS)
	$(CC) $(SRCS) -o $(EXE_NAME) $(FLAGS) $(LFLAGS) -O3 -I$(INCLUDE) 
clean:
	rm -f $(EXE_NAME)

run: 
	./$(EXE_NAME)