#
# Makefile for CSF Assignment 3
#


C_SRCS = cache_funcs.cpp cache_main.cpp cache_tests.cpp tctest.c
CFLAGS = -g -O0 -Wall -Wextra -pedantic -std=c++11 

LDFLAGS = -no-pie

%.o : %.cpp
	g++ $(CFLAGS) -g $<

all : csim

csim : cache_main.o
	g++ -o csim cache_main.o

cache_main.o: cache_main.cpp
	g++ $(CFLAGS) -c -o cache_main.o cache_main.cpp

# Use this target to create a zipfile that you can submit to Gradescope
.PHONY: solution.zip 
solution.zip :
	rm -f solution.zip 
	zip -9r $@ Makefile README.txt *.h *.cpp

clean :
	rm -f *.o a.out csim depend.mak solution.zip

depend.mak :
	touch $@

depend :
	gcc -M $(C_SRCS) > depend.mak

include depend.mak
