@Echo off
if defined __ goto :mouse
set __=.
darkbox -i | %0 %* | darkbox
set __=
pause>NUL
goto :eof

:mouse
echo -sh 0
set old_X=0
set old_Y=0

:loop
set /p i=
for /f "tokens=1,2,3,4" %%A in ("%i%") do (
    if %%A==m (
        set X=%%B
        set Y=%%C
        set C=%%D
        goto :draw_cur
    )
)
goto :loop

:draw_cur
if %C% EQU 0 set C=F

echo -gra %old_X% %old_Y% 32
echo -gcarw %X% %Y% 0x%C%0 32 0

set old_X=%X%
set old_Y=%Y%
goto :loop
