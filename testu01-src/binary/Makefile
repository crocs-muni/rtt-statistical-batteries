export LD_LIBRARY_PATH=/home/xobrati1/Desktop/rtt-statistical-batteries/testu01-src/install/lib
export LIBRARY_PATH=/home/xobrati1/Desktop/rtt-statistical-batteries/testu01-src/install/lib
export C_INCLUDE_PATH=/home/xobrati1/Desktop/rtt-statistical-batteries/testu01-src/install/include

CC=gcc
CFLAGS=-I. -O2 -static -ltestu01 -lprobdist -lmylib -lm
DEPS = FileStreamGen.h TestU01Settings.h Utils.h
OBJ = TestU01.o Utils.o TestU01Settings.o FileStreamGen.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

TestU01: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o 
