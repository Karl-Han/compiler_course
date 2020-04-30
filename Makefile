run: test_syntax
	./syntax_main.out

test_syntax: syntax
	g++ -c -g test_syntax.cpp
	g++ -o syntax_main.out test_syntax.o syntax.o graph.o

syntax: graph
	g++ -c -g syntax.cpp

graph:
	g++ -c -g graph.cpp
scanner: 
	g++ scanner.cpp -o scanner.out

# run: scanner
# 	./scanner ./textbook.cpp

clean:
	rm -rf *.out scanner
