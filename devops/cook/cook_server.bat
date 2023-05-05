@echo.off

call "%~dp0\..\config.bat"

"%RunUATPath%" BuildCookRun ^
-project="%ProjectPath%" ^
-Platform="%Platform%" ^
-serverconfig="%Configuration%" ^
-build -cook -skippackage -server -noclient