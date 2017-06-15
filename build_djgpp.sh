#!/bin/sh
exec_suffix=-dpmi_dos.exe
CC=i686-pc-msdosdjgpp-gcc

make CC=$CC exec_suffix=$exec_suffix darkbox darkbox_i darkbox_t makemnu
make upx exec_suffix=$exec_suffix

pushd extras
make CC=$CC exec_suffix=$exec_suffix
make postbuild upx exec_suffix=$exec_suffix
popd
