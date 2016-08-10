include ../../kernel.settings
CC=$(PREFIX)gcc
LD=$(PREFIX)ld
SRCS = $(shell find . -name '*.[cS]')
OBJS = $(addsuffix .o,$(basename $(SRCS)))
ASFLAGS = 
CFLAGS += -I. -fno-stack-protector -nostdinc -ffreestanding -std=c11 -fno-builtin -w -Werror -nostdlib -fpie
LDFLAGS = -T loader.ld
all: $(OBJS)
	$(LD) $(LDFLAGS) -o loader.elf $(OBJS) -L$(HOME)/opt/lib/gcc/$(libpath)/6.1.0/ -lgcc
	objcopy -O binary loader.elf loader.bin
	rm loader.elf

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
%.o: %.S
	$(CC) $(ASFLAGS) -c -o $@ $^
