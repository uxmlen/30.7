# gmake
CXX = g++
CXXFLAGS = -g -Wall 
SRCMODULES = thread_pool.cpp
OBJMODULES = $(SRCMODULES:.cpp=.o)

%.o: %.c %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

out: main.cpp $(OBJMODULES)
	$(CXX) $(CXXFLAGS) $^ -o $@
run: out
	./out

clean:
	rm -rf *.o out

.PHONY: all run clean
