-include ../unp_make.env

PROGS = dns_server dns_client addr_info

all: ${PROGS}

dns_server:	dns_server.o
		${CC} ${CXXFLAGS} ${CFLAGS} -o $@ dns_server.o ${LIBS}	

dns_client: dns_client.o
	     ${CC} ${CXXFLAGS} ${CFLAGS} -o $@ dns_client.o ${LIBS}

addr_info: addrinfo.o
	${CC} ${CXXFLAGS} ${CFLAGS} -o $@ addrinfo.o ${LIBS}

%.o: %.cc
	${CC} ${CXXFLAGS} ${CFLAGS} -c $<
clean: 
	rm -f ${PROGS} *.o
	
