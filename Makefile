# Makefile to create authenticated IPFS API #

BASE=.
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

INCLUDES = -I$(LIB_INCLUDE) -I$(INCDIR) -I$(GLEWYWLYD_LOC)

CFLAGS = -g -Wall -D_REENTRANT -pthread
#  $(GLEWLWYD_LOC) $(ULFIUS_LOC)

LIBS=-L$(LIB_LOCATION) -lc  -lgnutls -lulfius -lorcania -lssl -ljwt -ljansson -lcrypto -lsqlite3 -std=c99 -lyder  -lcurl 
LDFLAGS=-pthread
_DEPS = jsonparser.h restserver.h ulfiusexample.h queue.h  dbcache.h   cryptreserveserver.h curlipfsclient.h cryptreservesecureapi.h jwthelper.h staticfilecallback.h filecache.h
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))
_SOURCES=  dat/jsonparser.c comm/restserver.c lib/queue.c dat/dbcache.c api/cryptreserveserver.c comm/cryptreservesecureapi.c encr/jwthelper.c comm/websocket.c comm/curlipfsclient.c dat/filecache.c
SOURCES= $(patsubst %.c,$(SRCDIR)/%.c,$(_SOURCES))
_OBJECTS = $(patsubst %.c,%.o,$(notdir $(_SOURCES)))
OBJECTS = $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(_SOURCES)))
#OBJECTS = $(patsubst %.c,%.o,$(notdir $(_SOURCES)))

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

$(EXE): $(_OBJECTS)
	@echo $(CC) LINKER: [$(notdir $%)] '->' [$(notdir $@)]
	$(CC) $(CFLAGS) -o $(EXE) $(_OBJECTS) $(LIBS) 
	@echo "Released $(EXE) Successfully!"

$(_OBJECTS) : $(SOURCES) directories
	@echo $(CC): [$<] '->' [$@]
	@$(CC) -c $< $(CFLAGS) $(INCLUDES)  $(SOURCES)
	#-o $@ 

directories :
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

clean: 
	rm -rf $(OBJECTS)
	rm -rf $(BINDIR)
	rm -rf $(OBJDIR)
