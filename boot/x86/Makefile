include ../../kernel.settings
CC=$(PREFIX)gcc
LD=$(PREFIX)ld
SRCS = $(shell find . -name '*.[cS]')
OBJS = $(addsuffix .o,$(basename $(SRCS)))
ASFLAGS = -m32
CFLAGS += -m32 -fno-stack-protector -nostdinc -ffreestanding -std=c11 -fno-builtin -w -Werror -nostdlib -fpie
LDFLAGS = -T loader.ld
all: $(OBJS)
	$(LD) $(LDFLAGS) -o loader.bin $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
%.o: %.S
	$(CC) $(ASFLAGS) -c -o $@ $^