@Echo off
setlocal enabledelayedexpansion
set CFLAGS=-O2 -s -Ilib -DWIN32

set PATH=%CD%\tcc;%PATH%
mkdir bin 2>nul

set darkbox_src=darkbox\darkbox.c lib\core.c lib\core_i.c
set ptnio_src=lib\sthread.c lib\znsock.c lib\nms.c lib\tiny_assert.c

for %%A in (ptnio\*.c ptnio\server\*.c ptnio\server\commands\*.c ptnio\client\*.c) do (
    set ptnio_src=!ptnio_src! %%A
)

set old_prompt=%prompt%

prompt ^>
@echo on

tcc %CFLAGS% -o bin\darkbox.exe %darkbox_src%
tcc %CFLAGS% -DAVOID_POINTER_WARNING -lkernel32 -lws2_32 -o bin\ptnio.exe %ptnio_src%

copy bin\*.exe tests\

@echo off
prompt %old_prompt%

pause
