-include ../unp_make.env

PROGS = echo_server echo_client

all: ${PROGS}

echo_server:	echo_server.o
		${CC} ${CXXFLAGS} ${CFLAGS} -o $@ echo_server.o ${LIBS}	

echo_client: echo_client.o
	     ${CC} ${CXXFLAGS} ${CFLAGS} -o $@ echo_client.o ${LIBS}

%.o: %.cc
	${CC} ${CXXFLAGS} ${CFLAGS} -c $<
clean: 
	rm -f ${PROGS} *.o
	
