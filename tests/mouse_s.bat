@Echo off
if defined __ goto :mouse
set __=.
call %0 %* | darkbox
set __=
pause>NUL
goto :eof

:mouse
echo -sh 0
set old_X=0
set old_Y=0

:loop
for /f "tokens=1,2,3" %%A in ('darkbox -y') do (
    set X=%%A
    set Y=%%B
    set C=%%C
)

if %C% EQU 0 set C=F

echo -gra %old_X% %old_Y% 32
echo -gcarw %X% %Y% 0x%C%0 32 0

set old_X=%X%
set old_Y=%Y%
goto :loop
