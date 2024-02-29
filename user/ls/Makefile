# Standard tool makefile
CC=gcc
CXX=g++
AS=nasm
LD=ld

CFLAGS=-Wall -Wextra -Werror -std=c99 -g
CXXFLAGS=-Wall -Wextra -Werror -std=c++11 -g
ASFLAGS=-f elf64 -g
CFLAGS+=-ffreestanding -nostdlib -fno-builtin -fno-stack-protector
CXXFLAGS+=-ffreestanding -nostdlib -fno-builtin -fno-stack-protector
# Link with ../../libc/libc.a
LDFLAGS=-L../../libc -nostdlib -lc

BINARY_NAME=ls

SRC=$(shell find . -name '*.c' -or -name '*.cpp' -or -name '*.s')

OBJ=$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.s,%.o,$(SRC))))

all: $(BINARY_NAME)

$(BINARY_NAME): $(OBJ)
	$(LD) -T linker.ld -o $(BINARY_NAME) $(OBJ) $(LDFLAGS)

clean:
	rm -f $(BINARY_NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp	
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

.PHONY: all clean