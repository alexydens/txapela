LIMINE_DIR=limine
SRC_DIR=src
INC_DIR=include
BIN_DIR=bin
OBJ_DIR=obj
LOG_DIR=log
ISO_DIR=iso
CONF_DIR=conf

CFLAGS = -ansi -Wall -Wextra -Wpedantic -Werror
CFLAGS += -ffreestanding
CFLAGS += -fno-stack-protector
CFLAGS += -fno-stack-check
CFLAGS += -fno-lto
CFLAGS += -fno-PIC
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -I$(INC_DIR) -include $(LIMINE_DIR)/limine.h
CFLAGS += -m64
CFLAGS += -march=x86-64
CFLAGS += -mno-80387
CFLAGS += -mno-mmx
CFLAGS += -mno-sse
CFLAGS += -mno-sse2
CFLAGS += -mno-red-zone
CFLAGS += -mcmodel=kernel

LDFLAGS = -nostdlib
LDFLAGS += -static
LDFLAGS += -z max-page-size=0x1000
LDFLAGS += -gc-sections
LDFLAGS += -T $(CONF_DIR)/linker.ld

C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
CXX_SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
ASM_SOURCES = $(wildcard $(SRC_DIR)/*.S)

OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES))
OBJECTS += $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CXX_SOURCES))
OBJECTS += $(patsubst $(SRC_DIR)/%.S,$(OBJ_DIR)/%.o,$(ASM_SOURCES))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.S | $(OBJ_DIR)
	$(AS) -o $@ $<

$(BIN_DIR)/txapela: $(OBJECTS) | $(BIN_DIR)
	$(LD) $(LDFLAGS) -o $@ $^

$(LIMINE_DIR):
	git clone https://github.com/limine-bootloader/limine.git \
		--branch=v8.x-binary --depth=1 $(LIMINE_DIR)
	$(MAKE) -C $(LIMINE_DIR)

$(BIN_DIR):
	mkdir -p $@
$(OBJ_DIR):
	mkdir -p $@
$(LOG_DIR):
	mkdir -p $@
$(ISO_DIR):
	mkdir -p $@

.PHONY: test clean

test: $(LIMINE_DIR) $(BIN_DIR)/txapela | $(ISO_DIR) $(LOG_DIR)
	mkdir -p $(ISO_DIR)/boot/limine
	mkdir -p $(ISO_DIR)/EFI/BOOT
	cp -v $(BIN_DIR)/txapela $(ISO_DIR)/boot
	cp -v $(CONF_DIR)/limine.conf $(ISO_DIR)/boot/limine
	cp -v \
		$(LIMINE_DIR)/limine-bios.sys    \
		$(LIMINE_DIR)/limine-bios-cd.bin \
		$(LIMINE_DIR)/limine-uefi-cd.bin \
		$(ISO_DIR)/boot/limine
	cp -v $(LIMINE_DIR)/BOOTX64.EFI $(ISO_DIR)/EFI/BOOT
	cp -v $(LIMINE_DIR)/BOOTIA32.EFI $(ISO_DIR)/EFI/BOOT
	xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin      \
		-no-emul-boot -boot-load-size 4 -boot-info-table         \
		--efi-boot boot/limine/limine-uefi-cd.bin                \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(ISO_DIR) -o $(BIN_DIR)/txapela.iso
	$(LIMINE_DIR)/limine bios-install $(BIN_DIR)/txapela.iso
	qemu-system-x86_64 -cdrom $(BIN_DIR)/txapela.iso -m 2G \
		-chardev stdio,id=char0,logfile=$(LOG_DIR)/serial_com1.log,signal=off \
		-serial chardev:char0
	#bochs -qf $(CONF_DIR)/bochsrc.txt

clean:
	rm -rf $(LIMINE_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	rm -rf $(LOG_DIR)
	rm -rf $(ISO_DIR)
