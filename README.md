# ross

A very simple OS for Raspberry Pi.

This README is quite bare. It exists so that I can remember how to set up a development environment.


# Dependencies

The scripts in this repository expect the following to be available on your PATH:

* mjkillough/qemu-rpi (QEMU fork with Raspberry Pi support)
* GCC ARM EABI toolchain

## Install qemu-rpi

To compile:

    $ brew install pkg-config glib automake libtool
    $ ./configure --target-list=arm-softmmu
    $ make
    $ make install

## Install embedded GCC ARM toolchain

Pre-compiled and source versions of this [are available](https://launchpad.net/gcc-arm-embedded/+download). Just download one and place it on your `PATH`.


# Develop / Run

To develop/run:

    $ make
    $ ./qemu.sh

To exit out of QEMU, hit: Ctrl + A, C, q, enter

## Debugging

To start QEMU with GDB stopped on the first instruction, pass `-s -S` to `./qemu.sh`.

To launch GDB, run `arm-none-eabi-gdb` from the root of the repo. (The repo contains a `.gdbinit` file, which sets up symbols and connects to a running QEMU session).

Useful tips:
* To enter the GDB curses UI, either press `C-x C-a` or pass `-tui`.
* When debugging ASM, `layout asm` and `stepi`/`si` are useful.


