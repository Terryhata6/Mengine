for %%O in (%*) do for /f "tokens=1,2 delims==" %%a in (%%O) do set "%%~a=%%~b"

echo **********************************************************************
echo SOLUTION_NAME: %SOLUTION_NAME%
echo CONFIGURATION: %CONFIGURATION%
echo GENERATOR: %GENERATOR%
echo PLATFORM_TOOLSET: %PLATFORM_TOOLSET%
echo ARCHITECTURE: %ARCHITECTURE%
echo SOURCE_DIRECTORY: %SOURCE_DIRECTORY%
echo VERBOSITY: %VERBOSITY%
echo **********************************************************************

if defined ARCHITECTURE (
    set "CMAKE_ARCHITECTURE=-A %ARCHITECTURE%"
) else (
    set "CMAKE_ARCHITECTURE="
)

if defined PLATFORM_TOOLSET (
    set "CMAKE_PLATFORM_TOOLSET=-T %PLATFORM_TOOLSET%"
) else (
    set "CMAKE_PLATFORM_TOOLSET="
)

if defined VERBOSITY (
    set "CMAKE_VERBOSITY=-- /verbosity:%VERBOSITY%"
) else (
    set "CMAKE_VERBOSITY"
)

if defined EXTERNAL_PDB_PATH (
    set "MENGINE_EXTERNAL_PDB=ON"
    set "MENGINE_EXTERNAL_PDB_PATH=%EXTERNAL_PDB_PATH%"
) else (
    set "MENGINE_EXTERNAL_PDB=OFF"
    set "MENGINE_EXTERNAL_PDB_PATH"
)

set "BUILD_TEMP_DIR=..\solutions\%SOLUTION_NAME%\%CONFIGURATION%"

@mkdir "%BUILD_TEMP_DIR%"
@pushd "%BUILD_TEMP_DIR%"

CMake -G "%GENERATOR%" %CMAKE_PLATFORM_TOOLSET% %CMAKE_ARCHITECTURE% -S "%SOURCE_DIRECTORY%" "-DCMAKE_BUILD_TYPE:STRING='%CONFIGURATION%'" "-DMENGINE_EXTERNAL_PDB=%MENGINE_EXTERNAL_PDB%" "-DMENGINE_EXTERNAL_PDB_PATH='%MENGINE_EXTERNAL_PDB_PATH%'"

@popd

@pushd "%BUILD_TEMP_DIR%"

CMake --build .\ -j 8 --config %CONFIGURATION% %CMAKE_VERBOSITY%

@popd