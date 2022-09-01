# Compiler and linker.
CXX = g++
# Sources, dependencies, objects and targets.
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst %.cpp,%.o,$(SRC))
TARGET := cookiec
INC = inc/
# Linker options.
LDDIR =
LDFLAGS +=
LDLIBS +=
# Debug
FLAGS += -g -Wall

$(TARGET): $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS) $(LDLIBS) $(FLAGS)

$(OBJ): %.o: %.cpp
	$(CXX) $(CPPFLAGS) -I$(INC) -c $^ -o $@ -std=c++2a $(FLAGS)

test:
	@echo "TEST: running ./cookiec -v"
	@./cookiec -v
	@echo "\n"
	@echo "TEST: running ./cookiec -h"
	@./cookiec -h
	@echo "\n"
	@echo "TEST: running ./cookiec -l -f test/srcfile"
	@./cookiec -l -f test/srcfile
.PHONY: test

clean:
	@-rm -f $(OBJ) cookiec
.PHONY: clean
