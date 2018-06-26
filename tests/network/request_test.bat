@Echo off
setlocal enabledelayedexpansion
title client

pushd ..
if defined DOS9_OS_TYPE (
  set PATH=!CD!:!PATH!
) else set PATH=!CD!;!PATH!
popd

:: Initialize everything
(
  socket -id:tcp_rq -n
  socket -id:tcp_rq -c new c
  socket -id:tcp_rq -c connect c 208.97.177.124 80
) || goto :error

:: Send request
socket -id:tcp_rq -c send c < request

:: Wait for response.
socket -id:tcp_rq -c poll -1 c > nul

:: Receive response.
socket -id:tcp_rq -c recv c
goto :end_loop

:error
echo error !

:end_loop
echo Terminated
pause
socket -id:tcp_rq -c exit
goto :eof
