@Echo off
setlocal enabledelayedexpansion
echo Start server...
socket -id:server -n -t:1

echo Create listner...
socket -id:server -c new listner 1
socket -id:server -c bind listner ::1 4884 || (echo !errorlevel! & goto :bind_fail)
socket -id:server -c listen listner 1

echo Waiting for connection...
socket -id:server -c accept listner client

echo.
echo Additional infos :
for /f "tokens=*" %%a in ('socket -id:server -c info client') do echo - %%a
echo.

echo Receive data...
(>out.txt socket -id:server -c nms_recv client) || goto :recv_error

echo Test finished !!

goto :end

:bind_fail
echo Failed to bind to port.
goto :end

:recv_error
echo Error occured while received data
goto :end

:end
socket -id:server -c exit
echo PS: You can remove $socket_* or .socket_* files.
pause
goto :eof
