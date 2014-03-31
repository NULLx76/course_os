include $(CURDIR)/../config.mk

TOOLCHAIN_PATH:=$(CURDIR)/../$(TOOLCHAIN_DIR)/$(BARE_METAL_TARGET)/bin
CC:=$(TOOLCHAIN_PATH)/$(BARE_METAL_TUPLE)-gcc
AS:=$(TOOLCHAIN_PATH)/$(BARE_METAL_TUPLE)-as
LD:=$(TOOLCHAIN_PATH)/$(BARE_METAL_TUPLE)-ld
OBJCOPY:=$(TOOLCHAIN_PATH)/$(BARE_METAL_TUPLE)-objcopy
MKIMAGE:=$(CURDIR)/../u-boot/u-boot-$(UBOOT_VERSION)/tools/mkimage

OBJS=start.o uart.o startup.o

all: course_os.img

run: course_os.img
	./offset.sh $(UBOOT_VERSION)
	${QEMU} -M versatilepb -m 128M -nographic -kernel course_os.img

# This takes the images and concatenates them to flash storage, which is what gets booted now
run2: flash.bin
	${QEMU} -M versatilepb -m 128M -nographic -kernel flash.bin

flash.bin: kernel.img
	$(MKIMAGE) -A arm -C none -O linux -T kernel -d kernel.bin -a 0x00010000 -e 0x00010000 kernel.img2
	dd if=/dev/zero of=flash.bin bs=1 count=6M
	dd if=../u-boot/u-boot-$(UBOOT_VERSION)/u-boot.bin of=flash.bin conv=notrunc bs=1
	dd if=kernel.img2 of=flash.bin conv=notrunc bs=1 seek=2M

kernel.elf: $(OBJS)
	$(LD) -T kernel.ld -Map=kernel.map $(OBJS) -o kernel.elf

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary kernel.elf kernel.bin

kernel.img: kernel.bin
	$(MKIMAGE) -A arm -C none -O linux -T kernel -d kernel.bin -a 0x00100000 -e 0x00100000 kernel.img

course_os.img: kernel.img 
	cat ../u-boot/u-boot-$(UBOOT_VERSION)/u-boot.bin kernel.img > course_os.img

startup.o: startup.s
	$(AS) -mcpu=arm1176jzf-s -g startup.s -o startup.o

%.o: %.c
	$(CC) -Iinclude -c -mcpu=arm1176jzf-s $< -o $@

clean: 
	rm -f *.o
	rm -f *.img
	rm -f *.elf
	rm -f *.bin
	rm -f *.map