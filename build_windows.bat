cd /d %~dp0

nmake /f Makefile.mak

ren library\open_jtalk.dll %0_open_jtalk.dll
cp library\%0_open_jtalk.dll %0_open_jtalk.dll

echo Finish OpenJTalk library build task
