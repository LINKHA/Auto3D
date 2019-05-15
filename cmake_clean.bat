@echo off
pushd %~dp0

:: Clean the CMake cache
if exist Build\CMakeCache.txt. del /F Build\CMakeCache.txt

:: Clean CMakeFiles directory
if exist Build\CMakeFiles. rd /S /Q Build\CMakeFiles

popd
