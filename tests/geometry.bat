@Echo off
if defined __ goto :geometry
set __=.
call %0 %* | darkbox
set __=
pause>NUL
goto :eof

:geometry
echo -rs

darkbox_t -b B 0 0 19 19

darkbox_t -l C 0 0 19 19
darkbox_t -l C 19 0 0 19

darkbox_t -b D 0 0 20 20 -h

darkbox_t -b C 0 20 21 21
darkbox_t -c A 10 30 9
darkbox_t -b 5 0 20 21 21 -h

echo -gcd 23 3 0xC "Ugly ? No ?"

echo -rq
goto :eof
