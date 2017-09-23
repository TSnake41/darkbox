@Echo off
if defined __ goto :bug
set __=.
call %0 %* | darkbox
set __=
pause>nul
goto :eof

:bug
echo -d "WHOOOOOOOOOPS"
echo - q hey, this is "-" and "q" with a space between !!
exit /b