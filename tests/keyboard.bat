@Echo off
if defined __ goto :keyboard
set __=.
darkbox -ik | call %0 %* | darkbox
set __=
pause>NUL
goto :eof

:keyboard
set /p i=
for /f "tokens=2" %%A in ("%i%") do (
    call :show_key %%A
)
goto :keyboard

:show_key
set /a r=%random% %% 15 + 1
echo -cdanw %r% "%~1 " %~1 0
goto :eof
