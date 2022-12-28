@echo off
set fname=File:
for %%i in (*.cpp) do call :concat %%i
echo Over
echo %fname%
pause
goto :eof

:concat
echo %1
set fname=%fname% %1
goto :eof