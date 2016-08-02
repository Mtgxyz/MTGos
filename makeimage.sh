#!/usr/bin/env bash
unamestr=`uname`
if [[ "$unamestr" != 'FreeBSD' ]]; then
    echo "This script only works on FreeBSD"
    exit -1
fi
echo "Cleaning build..."
gmake clean
echo "Building..."
gmake
echo "stripping"
strip --strip-debug loader.bin
for f in *.elf; do
    strip --strip-debug $f
done
echo "Extracting symbols (for bochs)"
nm loader.bin | grep " T " | awk '{ print $1" "$3 }' > loader.bin.sym
for f in *.elf; do
    nm $f | grep " T " | awk '{ print $1" "$3 }' > $f.sym
done
echo "Compressing..."
xz -vv9e mtgos.firm
for f in *.elf; do
    xz -vv9e $f
done
echo "How many kilobytes should the image be?"
read length
echo "OK. Creating image..."
dd if=/dev/zero of=mtgos.img bs=1K count=$length
echo "Enter your password for the remaining part of the executable."
sudo id
echo "Mounting image..."
dev=$(sudo mdconfig -a -t vnode mtgos.img)
echo "Partitioning image..."
sudo gpart create -s MBR $dev
sudo gpart add -t freebsd -b 512 ${dev}
echo "Formatting partition..."
sudo newfs_msdos /dev/${dev}s1
echo "Mounting image..."
mkdir mount
sudo mount -t msdosfs /dev/${dev}s1 mount
echo "Installing grub... (May take some time)"
sudo grub-install --target=i386-pc --boot-directory=mount /dev/$dev --compress=xz --install-modules="normal part_msdos fat multiboot biosdisk xzio" --modules="normal part_msdos fat multiboot biosdisk xzio" --locales="" --force
echo "Copying files..."
sudo mv loader.bin mtgos.firm.xz *.elf.xz mount
echo "Creating grub.cfg"
cat > grub.cfg << "EOF"
menuentry "MTGos" {
    multiboot /loader.bin
    module /mtgos.firm.xz
    module /dsp_txt.elf.xz
}
EOF
sudo mv grub.cfg mount/grub
echo "Unmounting everything"
sync
sleep 0.5
df -h mount
sudo umount mount
sudo mdconfig -d -u ${dev#md}
echo "Finalizing..."
rm -rf mount
echo "Done!"