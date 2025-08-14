CXX ?= g++
CXX_FLAGS := -c -Wall -ansi -pedatic -std=c++20

http_server: main.o
	$(CXX) -o http_server main.o

main.o: main.cpp server.cpp
	$(CXX) $(CXX_FLAGS) main.cpp

clean:
	/bin/rm -f *~ *.0 http_server main.o

.PHONY: clean
