@Echo off
if defined __ goto :damier
set __=.
call %0 %* | darkbox
set __=
pause>nul
goto :eof

:damier
setlocal enabledelayedexpansion
set l=10
set h=10

set c0=0x10
set c1=0x20

for /l %%A in (1,1,%h%) do (
    echo -%l%cd
    for /l %%B in (1,1,%l%) do (
		if !c! EQU %c0% (
			set c=%c1%
		) else set c=%c0%
		
		echo !c! X
	)
	
	if !c! EQU %c0% (
		set c=%c1%
	) else set c=%c0%

    echo -rn
)

echo -rq
goto :eof