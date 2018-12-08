@Echo off
setlocal enabledelayedexpansion

pushd ..
if !DOS9_OS_TYPE!==*NIX (
	set PATH=!CD!:!PATH!
) else set PATH=!CD!;!PATH!
popd

ptnio -id:bin_test -n || goto :error

for %%A in (
  "new server"
  "bind server 127.0.0.1 25567"
  "listen server 1"

  "new client"
  "connect client 127.0.0.1 25567"

  "accept server socket"
) do (>nul call ptnio -id:bin_test -c %%~A || goto :error)

:: Send binary file
ptnio -id:bin_test -c nms_send client < bin_test || echo nms_send error (!errorlevel!)

:: Receive binary file
ptnio -id:bin_test -c nms_recv socket > bin_output || echo nms_recv error (!errorlevel!)

echo Finished, check bin_output
goto :end

:error
echo Error occured (%errorlevel%).
goto :end

:end
2>nul ptnio -id:bin_test -c exit
pause
goto :eof
