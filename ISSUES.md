# Known issues:
- the UART-16550A has some bugs for the AArch64 architecture. These arouse due
to the fact that the MMIO registers can be written to fine, but don't act
properly when read from, (at least the status ones don't). Because of this, the
line status register isn't check before read/write on AArch64 architectures,
however this workaround does introduce some issues.
