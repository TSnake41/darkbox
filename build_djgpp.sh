#!/bin/sh
exec_suffix=-dpmi_dos.exe
CC=i686-pc-msdosdjgpp-gcc

make CC=$CC exec_suffix=$exec_suffix
make upx exec_suffix=$exec_suffix
