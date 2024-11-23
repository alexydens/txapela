# Shell
SHELL=/bin/sh

# Directories already present
SRC_DIR=src
INC_DIR=include
CONF_DIR=conf
LINK_DIR=link

# Directories that may not be present
OBJ_DIR=obj
BIN_DIR=bin
ISO_DIR=iso

# Toolchain
# Available targets:
# - x86-grub
# - x86_64-grub
# - riscv32-uboot
# - riscv64-uboot
# - arm-uboot
# - arm64-uboot
# Default: x86_64-grub
TARGET=x86_64-grub
ifeq ($(TARGET),x86-grub)
	TRIPLE=i386-unknown-elf
	ARCH=x86
	BOOT=multiboot2
endif
ifeq ($(TARGET),x86_64-grub)
	TRIPLE=x86_64-unknown-elf
	ARCH=x86_64
	BOOT=multiboot2
endif
ifeq ($(TARGET),riscv32-uboot)
	TRIPLE=riscv32-unknown-elf
	ARCH=riscv32
	BOOT=uboot
endif
ifeq ($(TARGET),riscv64-uboot)
	TRIPLE=riscv64-unknown-elf
	ARCH=riscv64
	BOOT=uboot
endif
ifeq ($(TARGET),arm-uboot)
	TRIPLE=arm-unknown-elf
	ARCH=arm
	BOOT=uboot
endif
ifeq ($(TARGET),arm64-uboot)
	TRIPLE=aarch64-unknown-elf
	ARCH=arm64
	BOOT=uboot
endif
LINKER_SCRIPT=$(LINK_DIR)/linker-$(TARGET).ld
CC=clang -target $(TRIPLE)
AS=clang -target $(TRIPLE)
CXX=clang++ -target $(TRIPLE)
LD=ld.lld

# Compiler flags
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror
CFLAGS += -nostdinc
CFLAGS += -ffreestanding -fno-PIC -fno-lto
CFLAGS += -fno-stack-check -fno-stack-protector
CFLAGS += -I$(INC_DIR)/

# Linker flags
LDFLAGS = -T $(LINKER_SCRIPT)
LDFLAGS += -nostdlib -static -z max-page-size=0x1000

# Flags specific to target
ifeq ($(ARCH),x86)
	CFLAGS += -D__arch_x86__

	CFLAGS += -march=i386
	#CFLAGS += -mcmodel=large
	CFLAGS += -mno-red-zone # It might be better catch no support for these in the kernel itself.
	#CFLAGS += -mno-80387
	#CFLAGS += -mno-mmx -mno-avx -mno-sse -mno-sse2

	LDFLAGS += -m elf_i386
endif
ifeq ($(ARCH),x86_64)
	CFLAGS += -D__arch_x86_64__

	CFLAGS += -march=x86-64
	CFLAGS += -mcmodel=kernel
	CFLAGS += -mno-red-zone
	# It might be better catch no support for these in the kernel itself.
	#CFLAGS += -mno-80387
	#CFLAGS += -mno-mmx -mno-avx -mno-sse -mno-sse2

	LDFLAGS += -m elf_x86_64
endif
ifeq ($(ARCH),riscv32)
	CFLAGS += -D__arch_riscv32__

	CFLAGS += -march=rv32imac
	CFLAGS += -mabi=ilp32
	CFLAGS += -mno-relax
	CFLAGS += -mstrict-align

	LDFLAGS += -m elf32lriscv
	LDFLAGS += --no-relax
endif
ifeq ($(ARCH),riscv64)
	CFLAGS += -D__arch_riscv64__

	CFLAGS += -march=rv64imac
	CFLAGS += -mabi=lp64
	CFLAGS += -mno-relax
	CFLAGS += -mstrict-align

	LDFLAGS += -m elf64lriscv
	LDFLAGS += --no-relax
endif
ifeq ($(ARCH),arm)
	CFLAGS += -D__arch_arm__
	
	CFLAGS += -march=armv7-a
	# Probably better than expecting a certain type of FPU for 32-bit arm, as it
	# doesn't have the same standardized FPU support as AArch64.
	CFLAGS += -mfloat-abi=soft
	CFLAGS += -mfpu=none

	LDFLAGS += -m armelf
endif
ifeq ($(ARCH),arm64)
	CFLAGS += -D__arch_arm64__

	CFLAGS += -march=armv8-a
	CFLAGS += -mstrict-align

	LDFLAGS += -m aarch64elf
endif
ifeq ($(BOOT),multiboot2)
	CFLAGS += -D__boot_multiboot2__
endif
ifeq ($(BOOT),uboot)
	CFLAGS += -D__boot_uboot__
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
	@printf "\tCC\t$<\n"
	@$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR) 
	@printf "\tCXX\t$<\n"
	@$(CXX) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.S | $(OBJ_DIR) 
	@printf "\tAS\t$<\n"
	@$(AS) $(CFLAGS) -c $< -o $@

# Linking
$(BIN_DIR)/txapela.elf: $(OBJECTS) | $(OBJ_DIR) $(BIN_DIR) 
	@printf "\tLD\n"
	@for obj in $(OBJECTS) ; do \
    printf "\t\t$$obj\n" ; \
	done
	@$(LD) $(LDFLAGS) $(OBJECTS) -o $@

# Directories
$(BIN_DIR):
	mkdir -p $@
$(OBJ_DIR):
	mkdir -p $@

# ISO
$(BIN_DIR)/txapela.iso: $(BIN_DIR)/txapela.elf
ifeq ($(BOOT),multiboot2)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(CONF_DIR)/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	cp $(BIN_DIR)/txapela.elf $(ISO_DIR)/boot/txapela.elf
	grub-mkrescue -o $@ $(ISO_DIR)
endif
ifeq ($(BOOT),uboot)
	@echo "TODO: uboot"
endif

.PHONY: clean build test

test: build
ifeq ($(TARGET),x86-grub)
	qemu-system-i386 -cdrom $(BIN_DIR)/txapela.iso
else ifeq ($(TARGET),x86_64-grub)
	qemu-system-x86_64 -cdrom $(BIN_DIR)/txapela.iso
else
	@echo "ERROR: $(TARGET) Not fully implemented!"
	@false
endif

build: $(BIN_DIR)/txapela.iso

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	rm -rf $(ISO_DIR)
