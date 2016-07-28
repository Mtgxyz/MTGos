all: mbrcode.bin core.img

mbrcode.bin: stage1.S
	nasm -f bin -o $@ $^

core.img: stage2/start.S
	nasm -f bin -o $@ $^