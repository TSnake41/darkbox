@Echo off
if defined __ goto :mouse
set __=.
call %0 %* | darkbox
set __=
pause>NUL
goto :eof

:mouse
echo -sh 0

:loop
for /f "tokens=1,2,3" %%A in ('darkbox_i -y') do (
    set X=%%A
    set Y=%%B
    set C=%%C
)

if %C% EQU 0 set C=F
echo -gcarw %X% %Y% 0x%C%0 32 0
goto :loop
