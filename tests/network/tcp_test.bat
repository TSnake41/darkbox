@Echo off
setlocal enabledelayedexpansion
title client

pushd ..
if !DOS9_OS_TYPE!==*NIX (
	set PATH=!CD!:!PATH!
) else set PATH=!CD!;!PATH!
popd

ptnio -id:tcp_test -n || goto :error

for %%A in (
  "new server"
  "bind server 127.0.0.1 25567"
  "listen server 1"
  "new client"
  "connect client 127.0.0.1 25567"
  "accept server client_peer"
) do (>nul call ptnio -id:tcp_test -c %%~A || goto :error)

echo Hello World !! | ptnio -id:tcp_test -c send client
:loop
for /f "tokens=1,*" %%A in ('ptnio -id:tcp_test -c poll 50 client_peer') do goto :receive
goto :end_loop
:receive
ptnio -id:tcp_test -c recv client_peer true 1
darkbox -w 50
goto :loop
goto :end_loop

:error
echo error (%errorlevel%) !

:end_loop
echo.
echo Terminated
ptnio -id:tcp_test -c exit
pause>nul
goto :eof
