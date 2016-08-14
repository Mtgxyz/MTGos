include ../../../../kernel.settings
SRCS = $(shell find . -name '*.cpp' -o -name '*.[cS]')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

CPP = $(PREFIX)g++
CC = $(PREFIX)gcc
LD = $(PREFIX)g++

CFLAGS += -Wall -fno-stack-protector -nostdinc -Ic_include/ -I../../../../c_include -ffreestanding -std=c11 -fno-builtin -Werror -nostdlib -g -fPIC -pie
CPPFLAGS += -Wall -fno-stack-protector -nostdinc -std=c++14 -Iinclude/ -Ic_include/ -I../../../../c_include -I../../../../include -fno-rtti -fno-exceptions -ffreestanding -fno-builtin -Werror -nostdlib  -fno-use-cxa-atexit -Wextra -Wno-unused -g -fPIC -pie -Wno-reorder -fdump-class-hierarchy
LDFLAGS += -nostdlib -nodefaultlibs -nostdlib -fno-builtin -T ../../link.ld
all: $(OBJS)
	$(LD) $(LDFLAGS) -o ../../../../dsp_txt.elf $(OBJS) -lgcc
%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(OBJS)

.PHONY: clean all