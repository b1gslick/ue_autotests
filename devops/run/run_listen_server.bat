@echo.off

call "%~dp0\..\config.bat"

start "" "%GameExePath%" ?listen -WINDOWED -ResX=600 -ResY=1230 -WinX=10 -WinY=50