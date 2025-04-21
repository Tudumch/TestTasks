@echo off

:: The script makes a clean-up of the project by deleting folders with temporary files. 
:: Then recompile the project and generate a file for VS.


set CONFIG_FILE_NAME=config.bat

:: Change directory to current UE-project root
cd "%~dp0"
cd ".."
cd ".."
set PROJECT_DIR=%cd%
echo Project dir: %PROJECT_DIR%
set PLUGINS_DIR=%PROJECT_DIR%\Plugins

call %~dp0%CONFIG_FILE_NAME%

:: Removing temporary dirs and files in project dir
for %%a in (%DIRS_TO_REMOVE%) do (
    rmdir /S /Q %%a
)

for %%a in (%FILES_TO_REMOVE%) do (
   del %%a
)
echo Removing temporary files in project successfully complete!

:: Remove temporary dirs and files in marked plugin folders
setlocal EnableDelayedExpansion
for %%a in (%PLUGINS_NAMES%) do (
    
    set PLUGIN_FULL_PATH="%PLUGINS_DIR%\%%a"
    
    cd "!PLUGIN_FULL_PATH!"

    for %%i in (%DIRS_TO_REMOVE%) do (
        rmdir /S /Q %%i
        )

    for %%f in (%FILES_TO_REMOVE%) do (
        del %%f
        )
    )

echo Removing temporary files in plugins successfully complete!

:: Run UnrealBuildTool to generate project files
cd %PROJECT_DIR%
echo Current Path: %cd%

call "%RUN_BUILD_PATH%" "%PROJECT_PURE_NAME%"Editor "%PLATFORM%" Development "%UPROJECT%" -waitmutex

"%UE_ENGINE_PATH%\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="%UPROJECT%" -game -rocket -progress

:: Replacing VS Code workspace cpp-config for proper intellisence work
:: copy /Y "DevOps\CleanAndRegenerate\c_cpp_properties.json" ".vscode\c_cpp_properties.json"

pause
goto:EOF