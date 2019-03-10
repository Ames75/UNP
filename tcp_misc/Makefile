-include ../unp_make.env

PROGS = day_client

all: ${PROGS}

day_client: day_client.o
	     ${CC} ${CXXFLAGS} ${CFLAGS} -o $@ day_client.o ${LIBS}

%.o: %.cc
	${CC} ${CXXFLAGS} ${CFLAGS} -c $<
clean: 
	rm -f ${PROGS} *.o
	
