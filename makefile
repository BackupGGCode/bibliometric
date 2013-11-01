
all:merge bibliometric

.cpp.o:
	g++ -o $@ -c $<

bibliometric: bibliometric.o csvreader.o bibliometrics.o
	g++ -o $@ $^

merge:merge.o csvreader.o
	g++ -o $@ $^
