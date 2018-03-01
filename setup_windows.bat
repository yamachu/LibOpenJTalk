cd /d %~dp0

if %ARCH% == x86 (
    call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86
) else (
    call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64
)

git submodule update --init
cd hts_engine_API
setup_windows.bat

echo Setup Finished
