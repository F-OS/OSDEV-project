CC=gcc
CXX=g++
AS=nasm
LD=ld
AR=ar

# Start with standard flags
CFLAGS=-Wall -Wextra -Werror -std=gnu99 -g
CXXFLAGS=-Wall -Wextra -Werror -std=c++11 -g
ASFLAGS=-f elf64 -g
# Add flags to stop GCC from inserting code that'll interfere with the kernel or insert dependencies on the C library
CFLAGS+=-ffreestanding -nostdlib -fno-builtin -fno-stack-protector
CXXFLAGS+=-ffreestanding -nostdlib -fno-builtin -fno-stack-protector
# Add flags to reduce the size of the binary
CFLAGS+=-Os -fomit-frame-pointer -fno-exceptions -fno-asynchronous-unwind-tables
CXXFLAGS+=-Os -fomit-frame-pointer -fno-exceptions -fno-asynchronous-unwind-tables

LDFLAGS=-nostdlib -static -no-dynamic-linker

QEMU=qemu-system-x86_64
QEMUFLAGS=

#Disk
DISK=disk.img
QEMUFLAGS+=-drive file=$(DISK),format=raw

#QEMU Flags
QEMUMEM=256M
QEMUCPU=max
QEMUSMP=1
QEMUFLAGS+=-m $(QEMUMEM) -smp $(QEMUSMP) -serial stdio -cpu $(QEMUCPU)
# Enable KVM and other hardware acceleration to speed up the emulation
QEMUFLAGS+= -machine q35 -device intel-iommu -device intel-hda -device hda-duplex
# Debugging
QEMUFLAGS+=-d int -D qemu.log
QEMUFLAGS+=-no-reboot -no-shutdown
# Monitor on port 1234
QEMUFLAGS+=-monitor telnet::1234,server,nowait


KERNEL_DIR=kernel
KERNEL_SRC=$(shell find $(KERNEL_DIR) -name '*.c' -or -name '*.cpp' -or -name '*.s')
KERNEL_OBJ=$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.s,%.o,$(KERNEL_SRC))))

LIBC_DIR=libc
LIBC_SRC=$(shell find $(LIBC_DIR) -name '*.c' -or -name '*.cpp' -or -name '*.s')
LIBC_OBJ=$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.s,%.o,$(LIBC_SRC))))

USER_DIR=user
USER_TOOLS=$(shell find $(USER_DIR) -type d -not -name 'user' -exec basename {} \;)


all: kernel.bin $(USER_TOOLS) $(LIBC_DIR)/libc.a limine/bin/limine-bios.sys limine/bin/limine-bios-hdd.bin
	#Detach any loopback devices
	sudo kpartx -d $(DISK) || true
	sudo losetup -D || true
	sudo rm -f $(DISK)
	# Make a disk image
	dd if=/dev/zero of=$(DISK) bs=1M count=128
	parted -s $(DISK) mklabel msdos
	# BIOS Boot Partition
	parted -s $(DISK) mkpart primary fat32 1 100%
	# Attach the disk image to a loopback device
	sudo losetup -Pf $(DISK)
	# Format the disk image
	sudo mkfs.fat -F32 /dev/loop0p1
	# Mount the disk image
	mkdir -p mnt
	sudo mount /dev/loop0p1 mnt
	# Copy the kernel to the disk image
	sudo cp kernel.bin mnt
	sudo mkdir -p mnt/bin
	# Copy the user tools to the disk image
	for tool in $(USER_TOOLS); do \
		sudo cp $(USER_DIR)/$$tool/$$tool mnt/bin; \
	done
	# Copy the libc to the disk image
	sudo cp $(LIBC_DIR)/libc.a mnt
	sudo cp limine.cfg limine/bin/limine-bios.sys limine/bin/limine-bios-hdd.bin mnt
	# Unmount the disk image
	sudo umount mnt
	# Install the limine bootloader
	sudo limine/bin/limine bios-install $(DISK)
	#Detach the loopback device
	sudo losetup -D

qemu: all
	$(QEMU) $(QEMUFLAGS)

kernel.bin: $(KERNEL_OBJ)
	$(LD) -T $(KERNEL_DIR)/linker.kernel $(LDFLAGS) -o kernel.bin $(KERNEL_OBJ)	

$(LIBC_DIR)/libc.a: $(LIBC_OBJ)
	$(AR) rcs $(LIBC_DIR)/libc.a $(LIBC_OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<


$(USER_TOOLS): $(LIBC_DIR)/libc.a
	$(MAKE) -C $(USER_DIR)/$@

clean:
	rm -f kernel.bin $(KERNEL_OBJ) $(LIBC_OBJ)
	for tool in $(USER_TOOLS); do \
		$(MAKE) -C $(USER_DIR)/$$tool clean; \
	done
	rm -f $(LIBC_DIR)/libc.a
	sudo umount mnt || true
	sudo kpartx -d $(DISK) || true
	sudo losetup -D || true
	sudo rm -f $(DISK)
	sudo rm -rf mnt

limine/bin/limine-bios.sys limine/bin/limine-bios-hdd.bin:
	git submodule update --init --recursive
	(cd limine && git checkout trunk > /dev/null 2>&1)
	(cd limine && ./bootstrap > /dev/null 2>&1)
	(cd limine && ./configure --enable-bios > /dev/null 2>&1)
	(cd limine && make -j16 > /dev/null 2>&1)
	[ -f limine/bin/limine-bios.sys ] || (echo "limine/bin/limine-bios.sys not found" && exit 1)
	[ -f limine/bin/limine-bios-hdd.bin ] || (echo "limine/bin/limine-bios-hdd.bin not found" && exit 1)
	[ -f limine.cfg ] || (echo "limine.cfg not found" && exit 1)