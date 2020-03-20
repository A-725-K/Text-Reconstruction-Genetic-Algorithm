CC      = g++
DFLAGS  = -Wall -pedantic -DDEBUG
COMMON  = -o genetic.out
SRC_DIR = src
TRACE   = -DTRACE_ON
OPT     = -O2

.PHONY: clean debug trace

all: clean
	$(CC) $(SRC_DIR)/*.cpp main.cpp $(COMMON) $(OPT)

debug:
	$(CC) $(SRC_DIR)/*.cpp main.cpp $(COMMON) $(DFLAGS)

trace:
	$(CC) $(SRC_DIR)/*.cpp main.cpp $(COMMON) $(DFLAGS) $(TRACE)

clean:
	rm -f *.out
