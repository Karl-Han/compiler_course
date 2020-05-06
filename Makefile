debug: test_syntax
	lldb main

run: test_syntax
	./main

test_syntax: syntax libfort
	g++ -c -g -std=c++11 test_syntax.cpp
	g++ -std=c++11 -o main test_syntax.o syntax.o lib/fort.c graph.o vertex_edge.o

syntax: graph 
	g++ -c -g -std=c++11 syntax.cpp

# common: libfort
# 	g++ -c -g -std=c++11 common.cpp

libfort:
	g++ -c -g -std=c++11 lib/fort.hpp

graph: vertex_edge
	g++ -c -g graph.cpp

vertex_edge:
	g++ -c -g vertex_edge.cpp

scanner: 
	g++ scanner.cpp -o scanner.out

# run: scanner
# 	./scanner ./textbook.cpp

clean:
	rm -rf *.out scanner *.o
