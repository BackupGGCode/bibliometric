
all:merge bibliometrics
merge:merge.cpp
	g++ -o merge merge.cpp
bibliometrics:bibliometrics.cpp
	g++ -o bibliometrics bibliometrics.cpp

