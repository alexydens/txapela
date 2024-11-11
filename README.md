# Txapela, another kernel
## Usage
- `make tx-toolchain`: build the toolchain (compiles `gcc`, `binutils` and `bochs`
from source, so may take some time. The toolchain is required for utilities that
target the target architecture, rather than the host machine.
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
- `tx-toolchain/`: the toolchain.
- `limine/`: the [limine bootloader](https://github.com/limine-bootloader/limine).
### From build
- `obj/`: object files.
- `bin/`: binary files.
- `iso/`: the root directory of the ISO image.
### From test:
- `log/`: log files.
