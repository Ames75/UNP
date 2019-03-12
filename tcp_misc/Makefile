-include ../unp_make.env

PROGS = day_client day_server

all: ${PROGS}

day_client: day_client.o
	     ${CC} ${CXXFLAGS} ${CFLAGS} -o $@ day_client.o ${LIBS}

day_server: day_server.o
	${CC} ${CXXFLAGS} ${CFLAGS} -o $@ day_server.o ${LIBS}
%.o: %.cc
	${CC} ${CXXFLAGS} ${CFLAGS} -c $<
clean: 
	rm -f ${PROGS} *.o
	
