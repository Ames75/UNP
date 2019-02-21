
CC=g++
CXXFLAGS=-std=c++11
CFLAGS = -I../include -ggdb -O0 -D_REENTRANT -Wall
LIBS = ../lib/libunp.a -lpthread

PROGS = udp_echo_server udp_echo_client

all: ${PROGS}

udp_echo_server:	udp_echo_server.o
		${CC} ${CXXFLAGS} ${CFLAGS} -o $@ udp_echo_server.o ${LIBS}	

udp_echo_client: udp_echo_client.o
	     ${CC} ${CXXFLAGS} ${CFLAGS} -o $@ udp_echo_client.o ${LIBS}

%.o: %.cc
	${CC} ${CXXFLAGS} ${CFLAGS} -c $<
clean: 
	rm -f ${PROGS} *.o
	
