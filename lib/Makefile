CC=g++
CXXFLAGS=-std=c++11
CFLAGS = -I../include -ggdb -O0 -D_REENTRANT -Wall
MYLIB = libhgjunp.a
SRCS := $(wildcard *.cc) 
OBJS := $(patsubst %.cc, %.o, $(SRCS))

all : ${MYLIB}

${MYLIB} : ${OBJS}
	ar cr $@ $^

%.o : %.cc
	$(CC) $(CXXFLAGS) ${CFLAGS} -c $< 

clean:
	rm -f ${MYLIB} *.o
	
    
