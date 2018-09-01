@Echo off
setlocal enabledelayedexpansion
set CFLAGS=-O2 -s -Ilib -DWIN32

set PATH=%CD%\tcc;%PATH%
mkdir bin 2>nul

set darkbox_src=darkbox\darkbox.c lib\core.c lib\core_i.c
set darkbox_i_src=darkbox_i\darkbox_i.c lib\core_i.c
:: set makemenu_src=makemenu\makemenu.c makemenu\parser.c lib\core.c lib\fllist.c

set ptnio_src=lib\sthread.c lib\znsock.c lib\nms.c lib\tiny_assert.c

for %%A in (ptnio\*.c ptnio\server\*.c ptnio\server\commands\*.c ptnio\client\*.c) do (
    set ptnio_src=!ptnio_src! %%A
)

prompt ^>
@echo on

tcc %CFLAGS% -o bin\darkbox.exe %darkbox_src%
tcc %CFLAGS% -o bin\darkbox_i.exe %darkbox_i_src%
tcc %CFLAGS% -DAVOID_POINTER_WARNING -lkernel32 -lws2_32 -o bin\ptnio.exe %ptnio_src%
:: tcc %CFLAGS% -o bin\makemenu.exe %makemenu_src%

:: upx bin\*.exe
copy bin\*.exe tests\

@echo off
prompt

pause
