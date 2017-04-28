@Echo off
if defined __ goto :ascii
set __=.
call %0 %* | darkbox
set __=
pause>NUL
goto :eof

:ascii
echo -256a
for /l %%A in (0,1,255) do echo %%A
echo -rq
goto :eof
