@Echo off
setlocal enabledelayedexpansion

:: ACCURACY CONFIGURATION
set r_accuracy=165
set g_accuracy=165
set b_accuracy=165

set r_bright_factor=128
set g_bright_factor=128
set b_bright_factor=128
:: END

if not defined DOS9_OS (
    echo This script only work with Dos9,
    echo please take a version in ../dos9.
    echo.
    echo The Dos9 extentions are needed.
    pause>nul
    exit /b
)

if not exist "%~1" (
    echo File not found or unspecified.
    echo Usage : hex2darkbox.bat {file}
    goto :eof
)

set x=0
set y=0

set count=0

:: Reset buffer
for /f "tokens=1 delims==" %%A in ('set buffer_') do (
    set "%%A="
) 2>nul

:: Convert hex data to buffer.
for /f "tokens=* delims=" %%A in (%~1) do (
    for %%D in (%%A) do (
        set "c=%%D"

        set /a:i "r=0x!c:~0,2!"
        set /a:i "g=0x!c:~2,2!"
        set /a:i "b=0x!c:~4,2!"

        set c=0

        if [ !r! GTR !r_bright_factor! or !g! GTR !g_bright_factor! or !b! GTR !b_bright_factor! ] set c=8

        if !r! GTR !r_accuracy! set /a:i "c|=4"
        if !g! GTR !g_accuracy! set /a:i "c|=2"
        if !b! GTR !b_accuracy! set /a:i "c|=1"

        set /a:i "c=!c!<<4"

        set buffer_!x!_!y!=!c!

        set /a:i count+=1
        set /a:i x+=1
    )
    set /a lx=!x!-1
    set x=0
    set /a:i y+=1
)

set /a ly=%y%-1


:: Use buffer variables to generate a darkbox sprite code.
echo -%count%gca

for /l %%X in (0 1 !lx!) do (
    for /l %%Y in (0 1 !ly!) do (
        echo %%X %%Y !buffer_%%X_%%Y! 32
    )
)
goto :eof
