set PLATFORM=Win64
set PROJECT_PURE_NAME=""
set PLUGINS_NAMES=TDMCH_Tools

set DIRS_TO_REMOVE=Intermediate DerivedDataCache Binaries Build
set FILES_TO_REMOVE=*.sln *.code-workspace .vsconfig .ignore

:: -----------------------------------------------------------------
:: Check if the UE_ENGINE_PATH environment variable is set
if not defined UE_ENGINE_PATH (
    echo Error: UE_ENGINE_PATH environment variable is not set.
    echo Please set the UE_ENGINE_PATH variable to the Unreal Engine installation path.
    echo Example: setx UE_ENGINE_PATH "C:\Program Files\Epic Games\UE_5.3\Engine"

    pause
    exit /b 1
)
echo Unreal Engine Path: "%UE_ENGINE_PATH%"

set RUN_BUILD_PATH=%UE_ENGINE_PATH%\Build\BatchFiles\Build.bat

:: Change directory to the project directory
cd "%PROJECT_DIR%"

:: Find the first .uproject file in the directory
for /R %%i in (*.uproject) do (
    set "UPROJECT=%%i"

    :: If ProjectName was not set - parse it from UPROJECT path
    if %PROJECT_PURE_NAME%=="" set PROJECT_PURE_NAME=%%~ni
)
echo Uproject file: %UPROJECT%
echo Project pure name: %PROJECT_PURE_NAME%