#Makefile for Shell Homeworks
#


HEADERS = *.h
CXXSOURCE= *.cpp
CFLAGS = -std=c++11

UnixShell: $(CXXSOURCE) $(HEADERS)
	$(CXX) $(CFLAGS) -c $(CXXSOURCE)
	$(CXX) $(CFLAGS) -o UnixShell $(CXXSOURCE) -lreadline

.PHONY: clean
clean: 
	rm -rf *.o UnixShell *.txt


