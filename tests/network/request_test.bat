@Echo off
setlocal enabledelayedexpansion
title client

pushd ..
if defined DOS9_OS_TYPE (
  set PATH=!CD!:!PATH!
) else set PATH=!CD!;!PATH!
popd

(
  socket -id:tcp_rq -n
  socket -id:tcp_rq -c new c
  socket -id:tcp_rq -c connect c 208.97.177.124 80
) || goto :error

socket -id:tcp_rq -c send c < request

:: Wait for response.
darkbox -w 150
socket -id:tcp_rq -c recv c
goto :end_loop

:error
echo error !

:end_loop
echo Terminated
pause
socket -id:tcp_rq -c exit
goto :eof
