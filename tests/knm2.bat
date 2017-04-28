@Echo off
if defined __ goto :knm
set __=.
darkbox -i | %0 %* | darkbox
set __=
pause>NUL
goto :eof

:knm
echo -csh 0x00 0
set K=N/A

:loop
set /p i=
for /f "tokens=1,2,3,4" %%A in ("%i%") do (
    if %%A==m (
        set X=%%B
        set Y=%%C
        set C=%%D
        goto :draw_cur
    )
    if %%A==k (
        set K=%%B
        goto :draw_key
    )
)
goto :loop

:draw_cur
if %C% EQU 0 set C=F
echo -gcdw 0 0 0x9 "lastest key: %K%  " 0
echo -gcaw %X% %Y% 0x%C%0 32 0
goto :loop

:draw_key
echo -gcdw 0 0 0x9 "lastest key: %K%  " 0
goto :loop
