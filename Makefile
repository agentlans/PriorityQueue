
test: PriorityQueue.hpp Test.cpp test-cases.txt
	$(CXX) -o test Test.cpp
	./test

clean:
	rm -f test
