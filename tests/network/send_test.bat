@Echo off
setlocal enabledelayedexpansion
title client

pushd ..
if !DOS9_OS_TYPE!==*NIX (
	set PATH=!CD!:!PATH!
) else set PATH=!CD!;!PATH!
popd

set /p IP=IP :
set /p PORT=PORT :

ptnio -id:client -n || goto :error

for %%A in (
  "new socket"
  "connect socket %IP% %PORT%"
) do (>nul call ptnio -id:tcp_rq -c %%~A || goto :error)

:loop
set /p msg=Message to send :
(echo !msg! | ptnio -id:client -c nms_send socket) || goto :end_loop
echo Sent
goto :loop

:error
echo An error occured

:end_loop
echo Terminated
ptnio -id:client -c exit
goto :eof
