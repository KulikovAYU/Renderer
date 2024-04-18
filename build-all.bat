@ECHO OFF
rem Setting output directory when to build
SET OUTPUT_FOLDER=build

rem Setting Toolchain environment
SET CMAKE_GENERATOR_NAME=Ninja
SET CMAKE_GENERATOR_MAKE_PROGRAM="C:/Program Files/JetBrains/CLion 2023.3.4/bin/ninja/win/x64/ninja.exe"
SET CMAKE_C_COMPILER="C:/Program Files/LLVM/bin/clang.exe"
SET CMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang++.exe"
SET CMAKE_EXECUTABLE_PATH="C:/Program Files/CMake/bin/cmake.exe"

SET PLATFORM=%1
SET ACTION=%2
SET TARGET=%3

if "%ACTION%" == "build" (
    SET ACTION=all
    SET ACTION_STR=Building
    SET ACTION_STR_PAST=built
    SET DO_VERSION=yes
) else if "%ACTION%" == "clean" (
    SET ACTION=clean
    SET ACTION_STR=Cleaning
    SET ACTION_STR_PAST=cleaned
    SET DO_VERSION=no
) else (
    ECHO "Unknown action %ACTION%. Aborting" && EXIT
)


ECHO "%ACTION_STR% everything on %PLATFORM% (%TARGET%)..."
SET SOURCE_DIR=%~dp0
ECHO  Source directory: %SOURCE_DIR%

SET BUILD="%SOURCE_DIR%%OUTPUT_FOLDER%"
ECHO  Working directory: %BUILD%

ECHO  Cmake generator is %CMAKE_GENERATOR_NAME%
	
if "%ACTION%" == "all" (

	rem Building CMake script
	%CMAKE_EXECUTABLE_PATH% --no-warn-unused-cli ^
		-D CMAKE_EXPORT_COMPILE_COMMANDS=TRUE ^
		-D CMAKE_MAKE_PROGRAM=%CMAKE_GENERATOR_MAKE_PROGRAM% ^
		-D CMAKE_C_COMPILER=%CMAKE_C_COMPILER% ^
		-D CMAKE_CXX_COMPILER=%CMAKE_CXX_COMPILER% ^
		-S %SOURCE_DIR% ^
		-B %BUILD% ^
		-G %CMAKE_GENERATOR_NAME%
	IF ERRORLEVEL 1 (
		ECHO  Error: Failed to configure CMake project.
		EXIT
	)
		
	rem Assembyling the programm with generator
	cd %BUILD%
	%CMAKE_GENERATOR_MAKE_PROGRAM%
	IF ERRORLEVEL 1 (
		ECHO Error: Failed to build project with %CMAKE_GENERATOR_NAME%.
		EXIT
	)
) else if "%ACTION%" == "clean" (

	rem Cleaning the project
    IF EXIST %BUILD% (
        RD /S /Q %BUILD%
    ) ELSE (
        ECHO Directory %BUILD% does not exist.
    )
)

 ECHO All assemblies %ACTION_STR_PAST% successfully on %PLATFORM% (%TARGET%).