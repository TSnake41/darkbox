@Echo off
if defined __ goto :keyboard
set __=.
darkbox -i | call %0 %* | darkbox
set __=
pause>NUL
goto :eof

:keyboard
set /p i=
for /f "tokens=1,2" %%A in ("%i%") do (
	if %%A==k call :show_key %%B
)
goto :keyboard

:show_key
set /a r=%random% %% 15 + 1
echo -cdw %r% "%~1" 0
goto :eof
