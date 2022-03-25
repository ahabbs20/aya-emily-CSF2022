#
# Makefile for CSF Assignment 3
#


C_SRCS = cache_main.cpp Statistics.cpp Slot.cpp Set.cpp Cache.cpp
CFLAGS = -g -O2 -Wall -Wextra -pedantic -std=c++11 #think it was 02 when submitting, O0 when testing but not 100% sure lmao

LDFLAGS = -no-pie

all : csim

csim : cache_main.o Statistics.o Cache.o Set.o Slot.o
	g++ -o csim cache_main.o Statistics.o Cache.o Set.o Slot.o

cache_main.o: cache_main.cpp
	g++ $(CFLAGS) -c -o cache_main.o cache_main.cpp

Statistics.o: Statistics.cpp
	g++ $(CFLAGS) -c -o Statistics.o Statistics.cpp

Cache.o: Cache.cpp
	g++ $(CFLAGS) -c -o Cache.o Cache.cpp

Set.o: Set.cpp
	g++ $(CFLAGS) -c -o Set.o Set.cpp

Slot.o: Slot.cpp
	g++ $(CFLAGS) -c -o Slot.o Slot.cpp

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
