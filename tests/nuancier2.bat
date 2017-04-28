@Echo off
if defined __ goto :nuancier2
set __=.
call %0 %* | darkbox
set __=
pause>NUL
goto :eof

:nuancier2
setlocal enabledelayedexpansion

echo -256cd
for /l %%A in (0,1,255) do echo %%A X

echo -rq
goto :eof
