#
# Makefile for CSF Assignment 2
#
# You should not need to change anything in this makefile
#

C_SRCS = cache_funcs.cpp cache_main.cpp cache_tests.cpp tctest.c
CFLAGS = -g -O0 -Wall -Wextra -pedantic -std=c++11 

LDFLAGS = -no-pie

%.o : %.cpp
	g++ $(CFLAGS) -g $<

all : csim #cache_tests

#csim_tests : cache_funcs.o csim_tests.o tctest.o g++ -o csim_tests cache_funcs.o csim_tests.o tctest.o

csim : cache_funcs.o cache_main.o
	g++ -o csim cache_funcs.o cache_main.o

cache_main.o: cache_main.cpp
	g++ $(CFLAGS) -c -o cache_main.o cache_main.cpp

cache_funcs.o: cache_func.cpp	
	g++ $(CLFAGS) -c -o cache_funcs.o cache_func.cpp

#csim_tests.o: cache_tests.cpp tctest.h g++ $(CFLAGS) -c -o csim_tests.o cache_tests.cpp
	
# Use this target to create a zipfile that you can submit to Gradescope
.PHONY: solution.zip 
solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile README.txt *.h *.c *.cpp *.S

clean :
	rm -f *.o a.out cache_main cache_tests depend.mak solution.zip

depend.mak :
	touch $@

depend :
	gcc -M $(C_SRCS) > depend.mak

include depend.mak
