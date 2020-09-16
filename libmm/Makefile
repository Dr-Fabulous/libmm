D_BIN:=libmm.so
S_BIN:=libmm.a
SRC:=$(wildcard src/*.c)
INC:=./include
D_OBJ:=$(SRC:%.c=%.do)
S_OBJ:=$(SRC:%.c=%.o)

CC:=gcc
AR:=ar
COMMON_CFLAGS:=-c -std=c11 -I $(INC)
COMMON_LDFLAGS:=

ifneq ($(DEBUG),)
COMMON_CFLAGS+=-DDEBUG -g3
else
COMMON_CFLAGS+=-Os
endif

D_CFLAGS:=$(COMMON_CFLAGS) -fPIC
S_CFLAGS:=$(COMMON_CFLAGS)

D_LDFLAGS:=$(COMMON_LDFLAGS) -shared
S_LDFLAGS:=$(COMMON_LDFLAGS)

$(D_BIN): $(D_OBJ)
	$(CC) $(D_LDFLAGS) -o $@ $^

$(S_BIN): $(S_OBJ)
	$(AR) rcs $@ $^

%.do: %.c
	$(CC) $(D_CFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(S_CFLAGS) -o $@ $<

all: $(D_BIN) $(S_BIN)

clean:
	rm -f $(D_BIN) $(S_BIN) $(D_OBJ) $(S_OBJ)
