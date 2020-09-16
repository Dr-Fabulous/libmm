BIN:=unit-test
SRC:=$(wildcard src/*.c)
OBJ:=$(SRC:%.c=%.o)
INC:=../libmm/include
LIB:=../libmm/libmm.a

CC:=gcc
CFLAGS:=-c -std=c11 -I $(INC) -Wall -Wpedantic -DDEBUG -g3
LDFLAGS:=$(LIB)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ $< $(CFLAGS)

all: $(BIN)

clean:
	rm -f $(BIN) $(OBJ)

.PHONY: all clean
