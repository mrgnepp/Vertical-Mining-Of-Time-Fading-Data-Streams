EXE=vertime
CC=clang++
FLAGS=-c -Wall -std=c++11
CPP := $(wildcard Shamir/src/*.cpp)
OBJ := $(addprefix obj/,$(notdir $(CPP:.cpp=.o)))

all: $(EXE)

obj/%.o: VerticalTimeFading/%.cpp
	mkdir -p obj
	$(CC) $(FLAGS) $(INCLUDES) -o $@ $^

$(EXE): $(OBJ) obj/main.o
	$(CC) obj/main.o -o $(EXE) $(OBJ)

clean:
	rm -rf ./obj
	rm -f vertime
