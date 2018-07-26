@Echo off
setlocal enabledelayedexpansion

pushd ..
if defined DOS9_OS_TYPE (
	set PATH=!CD!:!PATH!
) else set PATH=!CD!;!PATH!
popd

(
  ptnio -id:bin_test -n
  ptnio -id:bin_test -c new server
  ptnio -id:bin_test -c bind server 127.0.0.1 25567
  ptnio -id:bin_test -c listen server 1

  ptnio -id:bin_test -c new client
  ptnio -id:bin_test -c connect client 127.0.0.1 25567

  ptnio -id:bin_test -c accept server socket > nul
) || goto :error

:: Send binary file
ptnio -id:bin_test -c nms_send client < bin_test || echo nms_send error (!errorlevel!)

:: Receive binary file
ptnio -id:bin_test -c nms_recv socket > bin_output || echo nms_recv error (!errorlevel!)

echo Finished, check bin_output
goto :end

:error
echo Error occured.
goto :end

:end
ptnio -id:bin_test -c exit
pause
goto :eof
