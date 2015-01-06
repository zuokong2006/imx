mkdir install
mkdir build

cd build
..\..\tools\cmake\win32\bin\cmake.exe -D WIN32=1 -G "Visual Studio 10" --build ..\

pause