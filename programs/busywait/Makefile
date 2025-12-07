FILES= ./build/busywait.o
INCLUDES= -I../stdlib/src
FLAGS= -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ${FILES}
	i686-elf-gcc -g -T ../shell/linker.ld -o ./busywait.elf -ffreestanding -O0 -nostdlib -fpic -g ${FILES} ../stdlib/stdlib.elf

build/busywait.o: ./busywait.c
	i686-elf-gcc ${INCLUDES} $(FLAGS) -std=gnu99 -c ./busywait.c -o ./build/busywait.o

clean:
	rm -rf ${FILES}
	rm -f ./busywait.elf
