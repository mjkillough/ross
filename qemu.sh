# Simple convenience script for starting QEMU.

# * Pass `-s -S` as arguments to this script to enable gdb support and stop on
#   the first instruction. (We pass it straight through to QEMU).
# * We pass both `-kernel` and `-initrd` as this causes QEMU to start at 0x8000,
#   rather than 0x1000.
# * We pass a bunch of nograph/stdio/console stuff as it makes it easier to
#   quit QEMU using the monitor (Cntrl + A, C, q, enter).

qemu-system-arm -kernel kernel.img -initrd kernel.img \
                -cpu arm1176 -m 256 -M raspi \
                -nographic -serial mon:stdio -append 'consolce=ttyS0' \
                $@
