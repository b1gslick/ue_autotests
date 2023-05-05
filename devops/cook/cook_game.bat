@echo.off

call "%~dp0\..\config.bat"

"%RunUATPath%" BuildCookRun ^
-project="%ProjectPath%" ^
-Platform="%Platform%" ^
-clientconfig="%Configuration%" ^
-build -cook -skippackage 