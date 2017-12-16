@Echo off
setlocal enabledelayedexpansion
title client

pushd ..
if defined DOS9_OS_TYPE (
	set PATH=!CD!:!PATH!
) else set PATH=!CD!;!PATH!
popd

set /p IP=IP :
set /p PORT=PORT :

(
	socket -id:client -n
	socket -id:client -c new socket
	socket -id:client -c connect socket %IP% %PORT%
) || goto :error

:loop
set /p msg=Message to send :
(echo !msg! | socket -id:client -c nms_send socket) || goto :end_loop
echo Sent
goto :loop

:end_loop
echo Terminated
socket -id:client -c exit
goto :eof
