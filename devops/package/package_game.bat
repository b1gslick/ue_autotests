@echo.off

call "%~dp0\..\config.bat"

"%RunUATPath%" BuildCookRun ^
-project="%ProjectPath%" ^
-Platform="%Platform%" ^
-serverconfig="%Configuration%" ^
-archiveddirectory="%ArchivePath%"
-build -cook -package -stage -archive -pak -allmaps -server -noclient