# Shell
SHELL=/bin/sh

# Directories already present
SRC_DIR=src
INC_DIR=include
CONF_DIR=conf

# Directories that may not be present
OBJ_DIR=obj
BIN_DIR=bin
ISO_DIR=iso
LOG_DIR=log
LIMINE_DIR=limine
OVMF_DIR=ovmf

# Toolchain
TARGET=x86_64-limine
#TARGET=riscv64-limine
#TARGET=aarch64-limine
LINKER_SCRIPT=$(CONF_DIR)/linker-$(TARGET).ld
ifeq ($(TARGET),x86_64-limine)
	TARGET_TRIPLE=x86_64-unknown-elf
	ARCH=x86_64
endif
ifeq ($(TARGET),riscv64-limine)
	TARGET_TRIPLE=riscv64-unknown-elf
	ARCH=riscv64
endif
ifeq ($(TARGET),aarch64-limine)
	TARGET_TRIPLE=aarch64-unknown-elf
	ARCH=aarch64
endif
CC=clang -target $(TARGET_TRIPLE)
AS=clang -target $(TARGET_TRIPLE)
CXX=clang++ -target $(TARGET_TRIPLE)
LD=ld.lld

# PROJECT STRUCTURE:
# Pre-existing:
# 	src/ 					-> source files
# 	include/			-> C/C++ header files
# 	conf/					-> configuration files and linker scripts
# Built seperately:
# 	limine/ 			-> bootloader (cloned from github, must build to use)
# From build:
# 	obj/					-> object files from compilation
# 	bin/ 					-> binary files from linking and building image
# 	iso/					-> the root directory of the ISO image
# From test:
# 	log/					-> log files generated by the test
# 	ovmf/					-> OVMF files

# Compiler flags
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror
CFLAGS += -nostdinc
CFLAGS += -I$(INC_DIR)/
CFLAGS += -ffreestanding
CFLAGS += -fno-stack-protector -fno-stack-check
CFLAGS += -fno-lto -fno-PIC
CFLAGS += -ffunction-sections -fdata-sections

# Linker flags
LDFLAGS = -nostdlib -static -z max-page-size=0x1000
LDFLAGS += -T $(LINKER_SCRIPT)

ifeq ($(TARGET),x86_64-limine)
	CFLAGS += -DTX_TARGET_X86_64_LIMINE
	CFLAGS += -m64
	CFLAGS += -march=x86-64
	CFLAGS += -mno-80387
	#CFLAGS += -mno-mmx -mno-sse -mno-sse2
	CFLAGS += -mno-mmx
	CFLAGS += -mno-red-zone
	CFLAGS += -mcmodel=kernel
	LDFLAGS += -m elf_x86_64
endif
ifeq ($(TARGET),riscv64-limine)
	CFLAGS += -DTX_TARGET_RV64_LIMINE
	CFLAGS += -march=rv64imac
	CFLAGS += -mabi=lp64
	CFLAGS += -mno-relax
	LDFLAGS += -m elf64lriscv
	LDFLAGS += --no-relax
endif
ifeq ($(TARGET),aarch64-limine)
	CFLAGS += -DTX_TARGET_AARCH64_LIMINE
	CFLAGS += -mgeneral-regs-only
	LDFLAGS += -m aarch64elf
endif

# Source files
C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
CXX_SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
ASM_SOURCES = $(wildcard $(SRC_DIR)/*.S)

# Object files
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_SOURCES))
OBJECTS += $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CXX_SOURCES))
OBJECTS += $(patsubst $(SRC_DIR)/%.S, $(OBJ_DIR)/%.o, $(ASM_SOURCES))

# Compiling
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR) 
	$(CXX) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.S | $(OBJ_DIR) 
	$(AS) $(CFLAGS) -c $< -o $@

# Linking
$(BIN_DIR)/txapela: $(OBJECTS) | $(OBJ_DIR) $(BIN_DIR) 
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

# OVMF
$(OVMF_DIR)/ovmf-code-$(ARCH).fd:
	mkdir -p $(OVMF_DIR)
	curl -Lo $@ https://github.com/osdev0/edk2-ovmf-nightly/releases/latest/download/ovmf-code-$(ARCH).fd
	case "$(ARCH)" in \
		aarch64) dd if=/dev/zero of=$@ bs=1 count=0 seek=67108864 2>/dev/null;; \
		riscv64) dd if=/dev/zero of=$@ bs=1 count=0 seek=33554432 2>/dev/null;; \
		esac

# Iso image
$(BIN_DIR)/txapela.iso: $(BIN_DIR)/txapela $(LIMINE_DIR)
	mkdir -p $(ISO_DIR)/boot/limine
	mkdir -p $(ISO_DIR)/EFI/BOOT
	cp -v $(BIN_DIR)/txapela $(ISO_DIR)/boot
	cp -v $(CONF_DIR)/limine.conf $(ISO_DIR)/boot/limine
ifeq ($(TARGET),x86_64-limine)
	cp -v \
		$(LIMINE_DIR)/limine-bios.sys \
		$(LIMINE_DIR)/limine-bios-cd.bin \
		$(LIMINE_DIR)/limine-uefi-cd.bin \
		$(ISO_DIR)/boot/limine
	cp -v $(LIMINE_DIR)/BOOTX64.EFI $(ISO_DIR)/EFI/BOOT/BOOTX64.EFI
	cp -v $(LIMINE_DIR)/BOOTIA32.EFI $(ISO_DIR)/EFI/BOOT/BOOTIA32.EFI
	xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin      \
		-no-emul-boot -boot-load-size 4 -boot-info-table         \
		--efi-boot boot/limine/limine-uefi-cd.bin                \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(ISO_DIR) -o $(BIN_DIR)/txapela.iso
endif
ifeq ($(TARGET),riscv64-limine)
	cp -v $(LIMINE_DIR)/limine-uefi-cd.bin $(ISO_DIR)/boot/limine/
	cp -v $(LIMINE_DIR)/BOOTRISCV64.EFI $(ISO_DIR)/EFI/BOOT/
	xorriso -as mkisofs -R -r -J \
		-hfsplus -apm-block-size 2048 \
		--efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(ISO_DIR) -o $(BIN_DIR)/txapela.iso
endif
ifeq ($(TARGET),aarch64-limine)
	cp -v $(LIMINE_DIR)/limine-uefi-cd.bin $(ISO_DIR)/boot/limine/
	cp -v $(LIMINE_DIR)/BOOTAA64.EFI $(ISO_DIR)/EFI/BOOT/
	xorriso -as mkisofs -R -r -J \
		-hfsplus -apm-block-size 2048 \
		--efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(ISO_DIR) -o $(BIN_DIR)/txapela.iso
endif
	$(LIMINE_DIR)/limine bios-install $(BIN_DIR)/txapela.iso

$(ISO_DIR):
	mkdir -p $@
$(BIN_DIR):
	mkdir -p $@
$(OBJ_DIR):
	mkdir -p $@
$(LOG_DIR):
	mkdir -p $@
:
	git clone https://github.com/alexydens/tx-toolchain.git 
	sh /scripts/build.sh
$(LIMINE_DIR):
	git clone https://github.com/limine-bootloader/limine.git \
		--branch=v8.x-binary --depth=1 $(LIMINE_DIR)
	$(MAKE) -C $(LIMINE_DIR)

.PHONY: clean full-clean build test

build: $(BIN_DIR)/txapela.iso

test: $(BIN_DIR)/txapela.iso | $(LOG_DIR) $(OVMF_DIR)/ovmf-code-$(ARCH).fd
ifeq ($(TARGET),x86_64-limine)
	qemu-system-x86_64 -cdrom $(BIN_DIR)/txapela.iso -m 4096M \
		-chardev stdio,id=char0,logfile=$(LOG_DIR)/serial_com1.log,signal=off \
		-serial chardev:char0
endif
ifeq ($(TARGET),riscv64-limine)
	qemu-system-riscv64 \
		-machine virt \
		-cpu rv64 \
		-device ramfb \
		-device qemu-xhci \
		-device usb-kbd \
		-device usb-mouse \
		-drive if=pflash,unit=0,format=raw,file=$(OVMF_DIR)/ovmf-code-$(ARCH).fd,readonly=on \
		-cdrom $(BIN_DIR)/txapela.iso \
		-chardev stdio,id=char0,logfile=$(LOG_DIR)/serial_com1.log,signal=off \
		-serial chardev:char0
endif
ifeq ($(TARGET),aarch64-limine)
	qemu-system-aarch64 \
		-machine virt \
		-cpu cortex-a72 \
		-device ramfb \
		-device qemu-xhci \
		-device usb-kbd \
		-device usb-mouse \
		-drive if=pflash,unit=0,format=raw,file=ovmf/ovmf-code-$(ARCH).fd,readonly=on \
		-cdrom $(BIN_DIR)/txapela.iso \
		-chardev stdio,id=char0,logfile=$(LOG_DIR)/serial_com1.log,signal=off \
		-serial chardev:char0
endif

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	rm -rf $(ISO_DIR)
	rm -rf $(LOG_DIR)
	rm -rf $(OVMF_DIR)
	rm -rf $(LIMINE_DIR)
