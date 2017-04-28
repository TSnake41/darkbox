@Echo off
if defined __ goto :d_nuancier
set __=.
call %0 %* | darkbox
set __=
pause>NUL
goto :eof

:d_nuancier
setlocal enabledelayedexpansion
for %%A in (0 1 2 3 4 5 6 7 8 9 A B C D E F) do (
    echo -16wcd
    for %%B in (0 1 2 3 4 5 6 7 8 9 A B C D E F) do (
        echo 50 0x%%A%%B Darkbox
    )
    echo -rnw 1000
)
echo -rq
goto :eof
