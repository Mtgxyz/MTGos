all: mbrcode.bin core.img
	cat mbrcode.bin core.img > disk.img
mbrcode.bin: stage1.S
	nasm -f bin -o $@ $^

core.img: 
	$(MAKE) -C stage2
	mv stage2/core.img .
.PHONY: core.img