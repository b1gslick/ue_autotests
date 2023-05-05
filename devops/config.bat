@echo.off

rem Engine params
set EnginePath=C:\JenkinsJob\UE
set EnginePathInstalledBuild=C:\JenkinsJob\UE\LocalBuilds\Engine\Windows
set FromCrush=C:\ue_pkgs\UE5_Windows_5.1.1-release_5ca9da8
@REM set VersionSelector=C:\JenkinsJob\UE\Engine\Binaries\Win64\UnrealVersionSelector.exe
set VersionSelector=C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe

rem !! Engine version for packaging !!
@REM set RunUATPath=%EnginePath%\Engine\Build\BatchFiles\RunUAT.bat
set RunUATPath=%FromCrush%\Engine\Build\BatchFiles\RunUAT.bat

rem Project params
set ProjectRoot=C:\Users\s_timokhin\_Projects\ForDevOps
set ProjectPureName=ForDevOps
set ProjectName=%ProjectPureName%.uproject
set ProjectPath=%ProjectRoot%\%ProjectName%

rem Build params
set Platform=Win64
set Configuration=Development
set ArchivePath=%ProjectRoot%\Build

rem Other params
set SourceCodePath=%ProjectRoot%\Source
set dirsToRemove=Intermediate DerivedDataCache Saved Binaries .vs Build
set filesToRemove=*.sln
set COPYRIGHT_LINE=//This is My project

rem Target params
set COPYRIGTH_LINE=// Copyright Epic games, Inc. All Rights Reserved.
set EXTRA_MODULE_NAMES="%ProjectPureName%"
set TargetTemplateFilePath=%ProjectRoot%\devops\targets\GameModule.Target.cs.template

rem Run
set ServerExePath=%ProjectRoot%/Build/WindowsServer/%ProjectPureName%Server.exe
set ClientExePath=%ProjectRoot%/Build/WindowsClient/%ProjectPureName%Client.exe
set GameExePath=%ProjectRoot%/Build/WindowsNoEditor/%ProjectPureName%.exe
