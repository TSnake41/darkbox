@Echo off
setlocal enabledelayedexpansion
title client

pushd ..
if defined DOS9_OS_TYPE (
  set PATH=!CD!:!PATH!
) else set PATH=!CD!;!PATH!
popd

(
  socket -id:tcp_test -n
  socket -id:tcp_test -c new server
  socket -id:tcp_test -c bind server 127.0.0.1 25567
  socket -id:tcp_test -c listen server 1
  socket -id:tcp_test -c new client
  socket -id:tcp_test -c connect client 127.0.0.1 25567
  socket -id:tcp_test -c accept server client_peer > nul
) || goto :error

(echo Hello World !!) | socket -id:tcp_test -c send client
for /l %%A in (1,1,14) do (
  socket -id:tcp_test -c recv client_peer true 1
  darkbox -w 50
)
goto :end_loop

:error
echo error !

:end_loop
echo.
echo Terminated
socket -id:tcp_test -c exit
pause>nul
goto :eof
