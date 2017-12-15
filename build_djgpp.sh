#!/bin/sh
export CC=i686-pc-msdosdjgpp-gcc

make darkbox exec_suffix=-dpmi_dos.exe
make darkbox_i exec_suffix=-dpmi_dos.exe
make darkbox_t exec_suffix=-dpmi_dos.exe

upx bin/*-dpmi_dos.exe

pushd extras
make all postbuild exec_suffix=-dpmi_dos.exe
upx bin/*-dpmi_dos.exe
popd
