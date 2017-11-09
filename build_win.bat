@Echo off
setlocal enabledelayedexpansion
set CFLAGS=-O2 -s -Ilib -DWIN32

set PATH=%CD%\tcc;%PATH%
mkdir bin 2>nul

set darkbox_src=darkbox\darkbox.c lib\core\core.c lib\core_i\core_i.c
set darkbox_i_src=darkbox_i\darkbox_i.c lib\core_i\core_i.c
set darkbox_t_src=darkbox_t\darkbox_t.c
set makemenu_src=makemenu\makemenu.c makemenu\parser.c lib\core\core.c lib\fllist\fllist.c

set socket_src=lib\sthread\sthread.c lib\socket\socket.c lib\ipc\socket_ipc.c lib\nms\nms.c

for %%A in (socket\*.c socket\server\*.c socket\server\commands\*.c socket\client\*.c) do (
    set socket_src=!socket_src! %%A
)

prompt ^>
@echo on

tcc %CFLAGS% -o bin\darkbox.exe %darkbox_src%
tcc %CFLAGS% -o bin\darkbox_i.exe %darkbox_i_src%
tcc %CFLAGS% -o bin\darkbox_t.exe %darkbox_t_src%
tcc %CFLAGS% -DAVOID_POINTER_WARNING -lkernel32 -lws2_32 -o bin\socket.exe %socket_src%
tcc %CFLAGS% -o bin\makemenu.exe %makemenu_src%

:: upx bin\*.exe
copy bin\*.exe tests\

@echo off
prompt

pause
