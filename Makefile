debug: test_syntax
	lldb syntax_main.out

run: test_syntax
	./syntax_main.out

test_syntax: syntax libfort
	g++ -c -g -std=c++11 test_syntax.cpp
	g++ -std=c++11 -o syntax_main.out test_syntax.o syntax.o lib/fort.c graph.o 

syntax: graph 
	g++ -c -g -std=c++11 syntax.cpp

# common: libfort
# 	g++ -c -g -std=c++11 common.cpp

libfort:
	g++ -c -g -std=c++11 lib/fort.hpp

graph:
	g++ -c -g graph.cpp
scanner: 
	g++ scanner.cpp -o scanner.out

# run: scanner
# 	./scanner ./textbook.cpp

clean:
	rm -rf *.out scanner *.o
