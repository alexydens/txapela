LIMINE_DIR=limine
SRC_DIR=src
INC_DIR=include
BIN_DIR=bin
OBJ_DIR=obj
LOG_DIR=log
ISO_DIR=iso
CONF_DIR=conf

$(LIMINE_DIR):
	git clone https://github.com/limine-bootloader/limine.git \
		--branch=v8.x-binary --depth=1 $(LIMINE_DIR)
	$(MAKE) -C $(LIMINE_DIR)

.PHONY: test clean

test: $(LIMINE_DIR)
	mkdir -p $(ISO_DIR)/boot/limine
	mkdir -p $(ISO_DIR)/EFI/BOOT
	cp -v $(BIN_DIR)/chapela $(ISO_DIR)/boot
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
		$(ISO_DIR) -o $(BIN_DIR)/chapela.iso
	$(LIMINE_DIR)/limine bios-install $(BIN_DIR)/chapela.iso

clean:
	rm -rf $(LIMINE_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	rm -rf $(LOG_DIR)
	rm -rf $(ISO_DIR)
