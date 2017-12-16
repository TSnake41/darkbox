@Echo off
setlocal enabledelayedexpansion
title server

pushd ..
if defined DOS9_OS_TYPE (
	set PATH=!CD!:!PATH!
) else set PATH=!CD!;!PATH!
popd

set /p IP=IP :
set /p PORT=PORT :

(
	socket -id:server -n
	socket -id:server -c new socket
	socket -id:server -c bind socket %IP% %PORT%
	socket -id:server -c listen socket 1
) || goto :error

:loop
socket -id:server -c accept socket client || goto :error

echo.
echo Client accepted

:client_loop
<nul set /p =Received :
(socket -id:server -c nms_recv client || goto :client_end_loop)
goto :client_loop
:client_end_loop
echo (end of stream)
echo Connection ended

socket -id:server -c free client
goto :loop

:error
echo.
echo Error occured :^(
socket -id:server -c exit
pause
goto :eof
