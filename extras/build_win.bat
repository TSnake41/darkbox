@Echo off
setlocal enabledelayedexpansion
set lib=..\lib
set CFLAGS=-O2 -I!lib! -DWIN32
set PATH=%CD%\..\tcc;%PATH%

mkdir bin 2>nul

set darkbox_i_src=darkbox_i\darkbox_i.c !lib!\core_i.c
set choice_src=choice\choice.c choice\parse_args.c choice\strcasechr.c !lib!\core.c !lib!\core_i.c

set old_prompt=%prompt%
prompt ^>
@echo on

tcc %CFLAGS% -o bin\choice_fr.exe %choice_src% -DLANG_FR
tcc %CFLAGS% -o bin\choice_de.exe %choice_src% -DLANG_DE
tcc %CFLAGS% -o bin\choice_en.exe %choice_src%

cmd /c fasm tcs_o\tcs_o.asm bin\tcs_o.exe

tcc %CFLAGS% -o bin\darkbox_i.exe %darkbox_i_src%
tcc %CFLAGS% -o bin\ctcs_o.exe tcs_o\ctcs_o.c
tcc %CFLAGS% -o bin\ctcs_o_core.exe tcs_o\ctcs_o_core.c !lib!\core.c

tcc %CFLAGS% -luser32 -o bin\cbatbox.exe cbatbox\cbatbox.c
tcc %CFLAGS% -o bin\cbatbox_core.exe cbatbox\cbatbox_core.c !lib!\core.c !lib!\core_i.c

copy bin\*.exe tests\

:: upx bin\*.exe

@echo off
prompt %old_prompt%

pause
