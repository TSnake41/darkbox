@Echo off
setlocal enabledelayedexpansion
set lib=..\lib
set CFLAGS=-O2 -I!lib! -DWIN32
set PATH=%CD%\..\tcc;%PATH%

mkdir bin 2>nul

set choice_src=choice\choice.c choice\parse_args.c choice\strcasechr.c !lib!\core.c !lib!\core_i.c

prompt ^>
@echo on

tcc %CFLAGS% -o bin\choice_fr.exe %choice_src% -DLANG_FR
tcc %CFLAGS% -o bin\choice_en.exe %choice_src%

:: upx bin\*.exe

@echo off
prompt

pause
