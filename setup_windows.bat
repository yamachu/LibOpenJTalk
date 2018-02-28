cd /d %~dp0

if %ARCH% == x86 (
    call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86
) else (
    call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64
)

git clone https://github.com/nemtrif/utfcpp.git
git clone https://github.com/yamachu/hts_engine_API.git
cd hts_engine_API
setup_windows.bat

echo Setup Finished
