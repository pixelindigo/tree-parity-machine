CC = g++
OBJS = main.cpp machine.h

tpm : $(OBJS)
	$(CC) -Wall --std=c++11 $(OBJS) -o tpm
