include ../kernel.settings
SRCS = $(shell find . -name '*.cpp' -o -name '*.[cS]')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

CPP = $(PREFIX)g++
CC = $(PREFIX)gcc
LD = $(PREFIX)g++

CFLAGS += -m32 -Wall -fno-stack-protector -nostdinc -Ic_include/ -I../c_include -ffreestanding -std=c11 -fno-builtin -Werror -nostdlib -g -fpie
CPPFLAGS += -m32 -Wall -fno-stack-protector -nostdinc -std=c++14 -Iinclude/ -Ic_include/ -I../c_include -I../include -fno-rtti -fno-exceptions -ffreestanding -fno-builtin -Werror -nostdlib  -fno-use-cxa-atexit -Wextra -Wno-unused -g -fno-pie -Wno-reorder
LDFLAGS = -nostdlib -nodefaultlibs -nostdlib -fno-builtin $(ARCHFLAGS) -T loader.ld
all: $(OBJS)
	$(LD) $(LDFLAGS) -o mtgos.elf $(OBJS)
%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(OBJS)

.PHONY: clean all