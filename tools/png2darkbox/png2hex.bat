@Echo off
(2>nul lua -v 0>CON) || call :lua_path
lua png2hex.lua %*
pause
goto :eof

:lua_path
:: Use lua version distributed in tools/lua
set PATH=%CD%\..\lua
2>nul lua -v || goto :no_lua
goto :eof

:no_lua
echo ERROR: No lua binary available.
pause
goto :eof
