#!/usr/bin/env bash
gmake clean
gmake loader.bin
gmake -C kernel
gmake clean
gmake -C modules
gmake clean
gmake -C modules subvar=11
mv kernel/mtgos.elf .
objcopy -O binary mtgos.elf mtgos.bin
gmake -C kernel subvar=11
objcopy -O binary kernel/mtgos.elf kernel/mtgos.bin
firmlink -O 08000000  -O 1FF80000 -E $(nm mtgos.elf | egrep ' _start$' | awk '{print $1}') -e $(nm kernel/mtgos.elf | egrep ' _start$' | awk '{print $1}') -o mtgos.firm mtgos.bin kernel/mtgos.bin
for f in *.elf; do
    strip --strip-debug $f
done
rm mtgos.bin kernel/mtgos.*
sudo mount_msdosfs /dev/da3s1 mount
sudo rm mount/{arm9loaderhax.bin,mtgos.firm}
sudo mv loader.bin mount/arm9loaderhax.bin
sudo mv mtgos.firm mount/mtgos.firm
sudo mv *.elf mount
sudo mv *.neun mount
sudo umount mount
echo "Remove SD card!"
