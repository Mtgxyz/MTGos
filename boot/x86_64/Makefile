include ../../kernel.settings
CC=$(PREFIX)gcc
LD=$(PREFIX)ld
SRCS = $(shell find . -name '*.[cS]')
OBJS = $(addsuffix .o,$(basename $(SRCS)))
ASFLAGS =
CFLAGS = -fno-stack-protector -nostdinc -ffreestanding -std=c11 -fno-builtin -w -Werror -nostdlib  
LDFLAGS = -T loader.ld -z max-page-size=0x1000
all: $(OBJS)
	$(LD) $(LDFLAGS) -z max-page-size=0x1000 -o loader.bin $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
%.o: %.S
	$(CC) $(ASFLAGS) -c -o $@ $^
