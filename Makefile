# DIRECTORIES
# The bootloader
LIMINE_DIR=limine
# The source files
SRC_DIR=src
# The header files
INC_DIR=include
# The binaries built
BIN_DIR=bin
# The object files
OBJ_DIR=obj
# The logs
LOG_DIR=log
# The image as it is being built
ISO_DIR=iso
# The configuration files (such as linker script, bootloader config, etc)
CONF_DIR=conf
# The cross compiler
CROSS_DIR=../toolchains/x86_64-elf/cross
CROSS_PREFIX=$(CROSS_DIR)/bin/x86_64-elf-

# TOOLS
# C compiler
CC=$(CROSS_PREFIX)gcc
# C++ compiler
CXX=$(CROSS_PREFIX)g++
# Linker
LD=$(CROSS_PREFIX)ld
# Assembler
AS=$(CROSS_PREFIX)as

# C COMILATION FLAGS
# Warnings and standard
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -Wno-unused
# Includes
CFLAGS += -I$(INC_DIR)
# Free standing
CFLAGS += -ffreestanding
# Stop GCC trying to protect stack
CFLAGS += -fno-stack-protector -fno-stack-check
# No position independent code
CFLAGS += -fno-PIC
# Seperate sections for functions and data
CFLAGS += -ffunction-sections -fdata-sections
# No FPU
CFLAGS += -mno-80387
# No SIMD
#CFLAGS += -mno-mmx -mno-sse -mno-sse2
# No red zone (area above stack for GCC)
CFLAGS += -mno-red-zone
# Higher half kernel memory model
CFLAGS += -mcmodel=kernel
# No optimisations
CFLAGS += -O0

# LINKER FLAGS
# No standard library linked in (I don't think the cross compiler has one tho)
LDFLAGS = -nostdlib
# No dynamic linking (there shouldn't be any anyway though)
LDFLAGS += -static
# Set the maximum page size
LDFLAGS += -z max-page-size=0x1000
# Remove unused sections
LDFLAGS += -gc-sections --print-gc-sections
# Linker script
LDFLAGS += -T $(CONF_DIR)/linker.ld

# SOURCE FILES
C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
CXX_SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
ASM_SOURCES = $(wildcard $(SRC_DIR)/*.S)

# OBJECT FILES
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES))
OBJECTS += $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CXX_SOURCES))
OBJECTS += $(patsubst $(SRC_DIR)/%.S,$(OBJ_DIR)/%.o,$(ASM_SOURCES))

# Compiling things...
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.S | $(OBJ_DIR)
	$(AS) -o $@ $<

# Linking the kernel
$(BIN_DIR)/txapela: $(OBJECTS) | $(BIN_DIR)
	$(LD) $(LDFLAGS) -o $@ $^

# Making the bootloader
$(LIMINE_DIR):
	git clone https://github.com/limine-bootloader/limine.git \
		--branch=v8.x-binary --depth=1 $(LIMINE_DIR)
	$(MAKE) -C $(LIMINE_DIR)

# The directories
$(BIN_DIR):
	mkdir -p $@
$(OBJ_DIR):
	mkdir -p $@
$(LOG_DIR):
	mkdir -p $@
$(ISO_DIR):
	mkdir -p $@

# Phony rules
.PHONY: test clean

# Build and test the bootloader
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
	#qemu-system-x86_64 -cdrom $(BIN_DIR)/txapela.iso -m 2G \
		#-chardev stdio,id=char0,logfile=$(LOG_DIR)/serial_com1.log,signal=off \
		#-serial chardev:char0
	../toolchains/bochs-x86_64/bochs-2.8/bochs -qf $(CONF_DIR)/bochsrc.txt

# Tidy up
clean:
	rm -rf $(LIMINE_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	rm -rf $(LOG_DIR)
	rm -rf $(ISO_DIR)
