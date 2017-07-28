cd /d %~dp0

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86

git clone https://github.com/yamachu/hts_engine_API.git
cd hts_engine_API
./setup_windows.bat

cd ..
@cd

nmake /f Makefile.mak

echo Finish OpenJTalk library build task
