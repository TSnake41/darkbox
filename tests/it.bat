@Echo off
setlocal enabledelayedexpansion
if defined __ goto :it
set __=.
darkbox -i | call %0 %* | darkbox
set __=
goto :eof

:it
echo -chs 0x00 0
echo -gcdr 0 4 0xA "Press F1 to quit (note: the window will be closed the next input)"

set X=N/A
set Y=N/A
set B=N/A
set K=N/A
call :update

:it_loop
darkbox -w 25
darkbox -h || goto :input

echo -gcdw 0 0 0x4 "No input : Yes" 0
goto :it_loop

:input
set /p i=
for /f "tokens=1,2,3,4" %%A in ("%i%") do (
    if %%A==k (
        set K=%%B
        if !K!==59 (
            echo -q
            exit
        )
    )
    if %%A==m (
        set X=%%B
        set Y=%%C
        set B=%%D
    )

    if %%A==q exit
)
call :update
darkbox -h || goto :input
goto :it_loop

:update
echo -g 0 0
echo -3cdn
echo 0x4 "No input : No "
echo 0x1 "Latest key press : %K%  "
echo 0x2 "Latest mouse event : x:%X% y:%Y% b:%B%    "
echo -w 0
goto :eof
