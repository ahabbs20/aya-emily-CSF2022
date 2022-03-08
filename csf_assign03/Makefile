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

csim : cache_main.o
	g++ -o csim cache_main.o

cache_main.o: cache_main.cpp
	g++ $(CFLAGS) -c -o cache_main.o cache_main.cpp

# Use this target to create a zipfile that you can submit to Gradescope
.PHONY: solution.zip 
solution.zip :
	rm -f solution.zip
<<<<<<< HEAD
	zip -9r $@ Makefile README.txt *.h *.cpp
=======
<<<<<<< HEAD
	zip -9r $@ Makefile README.txt *.h *.c *.cpp *.S
>>>>>>> 8670b60325321c868331520dc9cfe837e02d284f
>>>>>>> 6a6374568d4d1431a9e1a6e65bdd717c5f42d782

clean :
	rm -f *.o a.out csim depend.mak solution.zip

depend.mak :
	touch $@

depend :
	gcc -M $(C_SRCS) > depend.mak

include depend.mak
