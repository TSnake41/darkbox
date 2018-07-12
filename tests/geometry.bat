@Echo off
if defined __ goto :geometry
set __=.
call %0 %* | darkbox
set __=
pause>NUL
goto :eof

:geometry
echo -rs

:: Draw boxes backgrounds and borders
echo -4cb
echo  0xB0 1 1 18 18 0
echo  0xD0 0 0 20 20 1
echo  0xC0 1 21 19 19 0
echo  0x50 0 20 21 21 1

:: Draw top box diagonals
echo -c 0xC0 -2l
echo  0 0 19 19
echo  19 0 0 19

:: Draw bottom inline circle
echo -ci 0xA0 10 30 9

echo -gcd 23 3 0xC "Fancy graphics, maybe."

echo -rq
goto :eof
