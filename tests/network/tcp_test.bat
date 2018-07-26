@Echo off
setlocal enabledelayedexpansion
title client

pushd ..
if defined DOS9_OS_TYPE (
  set PATH=!CD!:!PATH!
) else set PATH=!CD!;!PATH!
popd

(
  ptnio -id:tcp_test -n
  ptnio -id:tcp_test -c new server
  ptnio -id:tcp_test -c bind server 127.0.0.1 25567
  ptnio -id:tcp_test -c listen server 1
  ptnio -id:tcp_test -c new client
  ptnio -id:tcp_test -c connect client 127.0.0.1 25567
  ptnio -id:tcp_test -c accept server client_peer > nul
) || goto :error

(echo Hello World !!) | ptnio -id:tcp_test -c send client
:loop
for /f "tokens=1,*" %%A in ('ptnio -id:tcp_test -c poll 50 client_peer') do goto :receive
goto :end_loop
:receive
ptnio -id:tcp_test -c recv client_peer true 1
darkbox -w 5
goto :loop
goto :end_loop

:error
echo error !

:end_loop
echo.
echo Terminated
ptnio -id:tcp_test -c exit
pause>nul
goto :eof
