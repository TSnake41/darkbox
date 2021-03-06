@Echo off
setlocal enabledelayedexpansion
title client

pushd ..
if !DOS9_OS_TYPE!==*NIX (
	set PATH=!CD!:!PATH!
) else set PATH=!CD!;!PATH!
popd

:: Initialize everything
ptnio -id:tcp_rq -n || goto :error

for %%A in (
  "new c"
  "connect c 208.97.177.124 80"
) do (>nul call ptnio -id:tcp_rq -c %%~A || goto :error)

:: Send request
ptnio -id:tcp_rq -c send c < request

:: Wait for response.
ptnio -id:tcp_rq -c poll -1 c > nul

:: Receive response.
ptnio -id:tcp_rq -c recv c
goto :end_loop

:error
echo error !

:end_loop
echo Terminated
pause
ptnio -id:tcp_rq -c exit
goto :eof
