@Echo off
setlocal enabledelayedexpansion
title server

pushd ..
if !DOS9_OS_TYPE!==*NIX (
	set PATH=!CD!:!PATH!
) else set PATH=!CD!;!PATH!
popd

set /p IP=IP :
set /p PORT=PORT :

ptnio -id:server -n || goto :error

for %%A in (
  "new socket"
  "bind socket %IP% %PORT%"
  "listen socket 1"
) do (>nul call ptnio -id:server -c %%~A || goto :error)

:loop
ptnio -id:server -c accept socket client || goto :error

echo.
echo Client accepted

:client_loop
<nul set /p =Received :
(ptnio -id:server -c nms_recv client || goto :client_end_loop)
goto :client_loop
:client_end_loop
echo (end of stream)
echo Connection ended

ptnio -id:server -c free client
goto :loop

:error
echo.
echo Error occured :^(
ptnio -id:server -c exit
pause
goto :eof
