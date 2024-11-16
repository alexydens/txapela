# Txapela, operating system kernel
## Usage
- `make build`: build the kernel iso image, stored in `bin/txapela.iso`.
- `make test`: test the kernel using the `qemu` emulator.
- `make test-logint`: test the kernel using the `qemu` emulator, logging interrupts.
- `make test-nox`: test the kernel using the `qemu` emulator, without a GUI.
- `make clean`: clean the directory of generated files.
- `make full-clean`: clean the directory to the state at clone.
### Specifying a target
- NOTE: for the `build` and `test` rules, you can specify a target.
- The following syntax is used: `make <build/test> TARGET="<target>"`.
- The following targets are available:
    - `x86_64-limine`: Limine bootloader, `x86_64` architecture.
    - `riscv64-limine`: Limine bootloader, `riscv64` architecture.
    - `aarch64-limine`: Limine bootloader, `aarch64` architecture.
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
