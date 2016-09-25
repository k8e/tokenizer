EXE=tokenizer
SRC=tokenizer.cpp main.cpp
CC=g++

all:
	$(CC) $(SRC) -o $(EXE)

clean:
	rm $(EXE)
