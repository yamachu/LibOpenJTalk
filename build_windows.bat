cd /d %~dp0

if %ARCH% == x86 (
    call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86
) else (
    call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64
)

nmake /f Makefile.mak

ren library\open_jtalk.dll %ARCH%_open_jtalk.dll

echo Finish OpenJTalk library build task
