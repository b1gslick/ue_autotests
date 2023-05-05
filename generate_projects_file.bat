@echo off
call "%~dp0devops\config.bat"

devops\misc\generate_project_file.bat "%FromCrush%" "%UBTRelativePath5%" "%VersionSelector%" "%ProjectName%"