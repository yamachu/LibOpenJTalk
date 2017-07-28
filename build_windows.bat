cd /d %~dp0

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86

nmake /f Makefile.mak

echo Finish OpenJTalk library build task
