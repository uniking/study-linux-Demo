all:$(cpp)
	i686-w64-mingw32-g++ testdll.cpp -shared -o testdll.dll -Wl,--out-implib,testdll.lib
	i686-w64-mingw32-g++ test.cpp -o test.exe testdll.lib
clean:
	rm test.exe testdll.dll testdll.lib
