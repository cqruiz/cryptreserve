EXAMPLE_INCLUDE=../include
LIB_LOCATION=/usr/local/lib
GLEW_LIB_LOC=/usr/local/lib/glewlwyd/usr
ULFIUS_LOC=../tools/ulfius/src
GLEWYWLYD_LOC=../tools/glewlwyd/src
INCLUDE=/usr/local/include
CFLAGS+=-c -g -Wall -L$(INCLUDE) $(EXAMPLE_INCLUDE)  $(ADDITIONALFLAGS) $(CPPFLAGS) $(GLEWLWYD_LOC) $(ULFIUS_LOC) -D_REENTRANT
LIBS=-lc -lgnutls -lulfius -lorcania -ljwt -ljansson -lcrypto -lsqlite3 -std=c99 -ljson-c  -lyder  

#-L$(ULFIUS_LOCATION)

IDIR = ../include
CC=gcc
CFLAGS+=-I$(IDIR) -g
DEPS = ../include/restserver.h ../include/u_example.h ../include/queue.h  ../include/dbcache.h ../include/jsonparser.h ../include/cryptreservesecureapi.h ../include/jwthelper.h
ODIR=../obj
LDIR=lib
LDFLAGS+=-pthread
SOURCES=cryptreserveserver.c comm/restserver.c lib/queue.c dat/dbcache.c dat/jsonparser.c comm/cryptreservesecureapi.c encr/jwthelper.c
OBJ=$(SOURCES:.c=.o)
OBJECTS= restserver.o queue.o cryptreserveserver.o dbcache.o jsonparser.o cryptreservesecureapi.o jwthelper.o
OBJS = $(patsubst %,$(ODIR)/%,$(OBJECTS))
EXE=../bin/CryptReserve


%.o: %.c
	$(CC) -c $(CFLAGS) $(SOURCES) 

$(EXE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS)  $(LIBS) -o $(EXE) 

#	$(CC) -v $(OBJS) $(LDFLAGS)  $(LIBS) -o $(EXE)

$(OBJECTS): $(DEPS)

debug:
	$(CC) $(CFDEBUG) $(SOURCES)

clean: 
	rm -f *.o $(EXE)


.PHONY: all static shared
