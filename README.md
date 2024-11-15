# Txapela, operating system kernel
## Usage
- `make build`: build the kernel iso image, stored in `bin/txapela.iso`.
- `make test`: test the kernel using the `bochs` emulator.
- `make clean`: clean the directory of generated files.
- `make full-clean`: clean the directory of generated files, including limine
and the toolchain.
## Directory structure
### Pre-existing
- `src/`: source files.
- `include/`: C/C++ header files.
- `conf/`: configuration files and linker scripts.
### Built once when first cloned
- `limine/`: the [limine bootloader](https://github.com/limine-bootloader/limine).
### From build
- `obj/`: object files.
- `bin/`: binary files.
- `iso/`: the root directory of the ISO image.
### From test:
- `log/`: log files.
## Include directory structure
- `include/ext`: headers that aren't written for the kernel specifically.
- `include/core`: some basic utilities and types, used everywhere.
