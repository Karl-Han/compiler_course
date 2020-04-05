scanner: 
	g++ scanner.cpp -o scanner

run: scanner
	./scanner ./textbook.cpp

clean:
	rm -rf a.out* scanner
