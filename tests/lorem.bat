@Echo off
if defined __ goto :lorem
set __=.
%0 %* | darkbox
echo.
set __=
pause>NUL
goto :eof

:lorem
setlocal enabledelayedexpansion
set text=Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
for /l %%p in (0,1,5) do for %%A in (%text%) do (
    set /a c=!random! %% 7 + 1
    echo -cd !c! "%%A "
)

echo -rq
goto :eof
