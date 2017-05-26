@Echo off
setlocal enabledelayedexpansion
if defined __ goto :it
set __=.
darkbox -i | call %0 %* | darkbox
set __=
pause>NUL
goto :eof

:it
echo -chs 0x00 0

set X=N/A
set Y=N/A
set B=N/A
set K=N/A
call :update


:it_loop
darkbox -w 25
darkbox -kbh || goto :input

echo -gcdw 0 0 0x4 "No input : Yes" 0
goto :it_loop

:input
set /p i=
for /f "tokens=1,2,3,4" %%A in ("%i%") do (
    if %%A==k (
        set K=%%B
    )
    if %%A==m (
        set X=%%B
        set Y=%%C
        set B=%%D
    )
)
call :update
darkbox -kbh || goto :input
goto :it_loop

:update
echo -g 0 0
echo -3cdn
echo 0x4 "No input : No "
echo 0x1 "Latest key press : %K%  "
echo 0x2 "Latest mouse event : x:%X% y:%Y% b:%B%    "
echo -w 0
goto :eof
