@Echo off
setlocal enabledelayedexpansion

if defined DOS9_OS (
    set __windows=
) else (
    set "PATH=%ProgramFiles(x86)%\VideoLAN\VLC\;%ProgramFiles%\VideoLAN\VLC\;%PATH%"
    set __windows=1
)

if "%1" EQU "" goto :help

if "%1" EQU "-l" (
    shift
    set __loop=1
) else set __loop=

(
    if not defined __loop 2>nul paplay --help && goto :paplay

    2>nul mplayer && goto :mplayer

    if !__windows!==1 (
        vlc -I rc --rc-quiet --no-one-instance-when-started-from-file --play-and-exit vlc://quit && goto :vlc
    ) else cvlc --no-one-instance-when-started-from-file vlc://quit && goto :vlc

    cscript && goto :vbs
) 2>&1 >nul
goto :eof

:help
echo darkbox_s - Sound Module for darkbox - Astie Teddy (TSnake41)
echo Usage : 
echo.  call darkbox_s.bat [-l] ^<file^> [name] [volume]
echo.
echo. -l: Loop as a music.
goto :eof

:vlc
set __vlc_flags=--play-and-exit
if not defined __loop set __vlc_flags=%__vlc_flags% --no-loop

if defined __windows (
    start /b vlc %__vlc_flags% -I rc --rc-quiet %1
) else sh -c "cvlc %__vlc_flags% %1 ^&"
goto :eof

:mplayer
set __mplayer_f=-vo dummy
if defined __loop (
    set __mplayer_f=%__mplayer_f% -loop 0
)

if defined __windows (
    start /b mplayer %__mplayer_f% %1 <nul 2>&1
) else (
    sh -c "mplayer %__mplayer_f% %1 ^&" <nul 2>&1
)
goto :eof

:paplay
sh -c "paplay %1 ^&"
goto :eof

:vbs
(
    echo Set Sound = CreateObject^("WMPlayer.OCX.7"^)
    echo Sound.URL = "%1"
    echo Sound.Controls.play
    echo do while Sound.currentmedia.duration = 0
    echo wscript.sleep 100
    echo loop
    echo wscript.sleep ^(int^(Sound.currentmedia.duration^)+1^)*1000
) >temp.vbs

start /b wscript temp.vbs
goto :eof