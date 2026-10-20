@echo off
setlocal
cd /d "%~dp0"

if not exist build (
	cmake -S . -B build || exit /b 1
)

cmake --build build --config Debug --parallel || (
	echo -- Build Failed --
	exit /b 1
)

echo -- Build Successful --

"build/Debug/CGui.exe"
exit /b %errorlevel%
