@Echo off
if defined __ goto :mouse
set __=.
darkbox -im | %0 %* | darkbox
set __=
pause>NUL
goto :eof

:mouse
echo -sh 0

:loop
set /p i=
for /f "tokens=2,3,4" %%A in ("%i%") do (
    set X=%%A
    set Y=%%B
    set C=%%C
    goto :draw_cur
)
goto :loop

:draw_cur
if %C% EQU 0 set C=F
echo -gcarw %X% %Y% 0x%C%0 32 0
goto :loop
