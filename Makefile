a.exe: Main.cpp API.cpp
	g++ -w -fcompare-debug-second Main.cpp API.cpp -o a.exe 