#
# Daniel A. Silva 820567716

CXX=g++
CXXFLAGS=-std=c++11 -g
LDFLAGS=-lpthread

wordcount : wordcount.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o wordcount $^

wordcount.o : wordcount.cpp

clean :
	rm *.o

