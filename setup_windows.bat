cd /d %~dp0

git submodule update --init
cd hts_engine_API
setup_windows.bat
cd ..

echo Setup Finished
