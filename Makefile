# Makefile to create authenticated IPFS API #

BASE:= ${CURDIR}


CC=gcc
INCDIR=$(BASE)/include
OBJDIR=$(BASE)/obj
SRCDIR=$(BASE)/src
BINDIR=$(BASE)/bin
NAME=cryptreserve
EXE=$(BINDIR)/$(NAME)

LIB_LOCATION=/usr/local/lib
GLEW_LIB_LOC=/usr/local/lib/glewlwyd/user
ULFIUS_LOC=$(BASE)/tools/ulfius/src
GLEWYWLYD_LOC=$(BASE)/tools/glewlwyd/src
LIB_INCLUDE=/usr/local/include
MBA_INCLUDE=/home/ed/dev/cryptreserve/tools/libmba/

INCLUDES = -I$(LIB_INCLUDE) -I$(INCDIR) -I$(GLEWYWLYD_LOC)

CFLAGS = -g -Wall -D_REENTRANT -pthread
#  $(GLEWLWYD_LOC) $(ULFIUS_LOC)

LIBS=-L$(LIB_LOCATION) -lc  -lgnutls -lulfius -lorcania -lssl -ljwt -ljansson -lcrypto -lsqlite3 -std=c99 -lyder  -lcurl -lmba
LDFLAGS=-pthread
_DEPS = jsonparser.h restserver.h ulfiusexample.h queue.h  dbcache.h   cryptreserveserver.h curlipfsclient.h cryptreservesecureapi.h jwthelper.h staticfilecallback.h filecache.h geth.h solidity.h
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))
_SOURCES=  dat/jsonparser.c comm/restserver.c lib/queue.c dat/dbcache.c api/cryptreserveserver.c comm/cryptreservesecureapi.c encr/jwthelper.c comm/websocket.c api/curlipfsclient.c dat/filecache.c api/geth.c api/solidity.c
SOURCES= $(patsubst %.c,$(SRCDIR)/%.c,$(_SOURCES))
_OBJECTS = $(patsubst %.c,%.o,$(notdir $(_SOURCES)))
#OBJECTS = $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(_SOURCES)))
OBJECTS = $(patsubst %.c,%.o,$(notdir $(_SOURCES)))

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CFLAGS += -D LINUX
endif
ifeq ($(UNAME_S),Darwin)
	CFLAGS += -D OSX
endif

#Make the Directories
#$(OBJECT): 
#	mkdir -p $(OBJECT)

# .PHONY: directories all clean

all: $(EXE)



$(EXE): $(OBJECTS)
	@echo   
	@echo   
	@echo $(CC) LINKER: [$(notdir $%)] '->' [$(notdir $@)]
	@echo   
	@echo LINKER: $(CC) $(CFLAGS) $(LIBS) -o  $@ $^
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) 
	@echo   
	@echo   

$(OBJECTS) : $(SOURCES) $(DEPS) directories
	@echo
	@echo
	@echo Building Object File: $(CC) OBJECTS: [$<] '->' [$@]
	@echo
	@echo
	@echo $(CC): [$(SOURCES) ] '->' [$@]
	@echo
	@echo
	@echo $(CC) -c -o $@   $(INCLUDES) $(SOURCES) $(CFLAGS) 
	@echo
	@echo
	@$(CC) -c  $(INCLUDES) $(SOURCES)  $(CFLAGS) $(DEPS)


directories :
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

clean: 
	rm -rf $(OBJECTS)
	rm -rf $(BINDIR)
	rm -rf $(OBJDIR)

.PHONY: default
