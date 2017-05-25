@Echo off
setlocal enabledelayedexpansion

socket -id:client -n -t:1

echo Create socket...
socket -id:client -c new socket 1

echo Connect to server...
socket -id:client -c connect socket ::1 4884 || goto :connect_error

echo Send file...
(socket -id:client -c nms_send socket < socket) || goto :send_error

echo Test finished !!
goto :end

:connect_error
echo Unable to connect to remote server.
goto :end

:send_error
echo Error occured while sending file.
goto :end

:end
socket -id:client -c exit
pause
exit
