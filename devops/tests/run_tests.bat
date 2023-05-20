@echo off

call "%~dp0..\config.bat"

rem build source
call "%RunUATPath%" BuildCookRun ^
-project="%ProjectPath%" ^
-platform="%Platform%" ^
-serverconfig="%Configuration%" ^
-build -cook

rem run tests
"%EditorPath%" "%ProjectPath%" -ExecCmds="Automation RunTests %DevOpsGame%;Quit" ^
-log -abslog="%TestOutputLogPath%" -nosplash -ReportOutputPath=%ReportOutputPath%
rem -nullRHI 
@REM -nullRHI not show UE


@REM SetFilter Stress
@REM AutomationCommandline.cpp

rem copy tests result
set TestsDir=%~dp0
set TestsDataDir=%~dp0data
robocopy "%TestsDataDir%" "%ReportOutputPath%" /E

rem start local server and show report
set Port=8081
set Localhost=http://localhost:%Port%

pushd "%ReportOutputPath%"
@REM start "" "%Localhost%"
@REM start "" "%Localhost%"
call http-server -p="%Port%"
popd