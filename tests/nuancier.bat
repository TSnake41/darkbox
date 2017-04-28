@Echo off
if defined __ goto :nuancier
set __=.
call %0 %* | darkbox
set __=
pause>NUL
goto :eof

:nuancier
setlocal enabledelayedexpansion
for %%A in (0 1 2 3 4 5 6 7 8 9 A B C D E F) do (
    echo -16cd
    for %%B in (0 1 2 3 4 5 6 7 8 9 A B C D E F) do (
        echo 0x%%A%%B Darkbox
    )
    echo -rn
)
echo -rq
goto :eof
