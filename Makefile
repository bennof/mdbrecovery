# Minimal C Makefile

## Config
INCLUDE_DIR=./src
SRC_DIR=./src
OBJ_DIR=./src
BIN_DIR=./bin

DEPS=
TARGET= mdbrecovery

# Compiler
CC=gcc
CFLAGS= -Wall -I$(INCLUDE_DIR) 
INCLUDE=

# Linker
LD=gcc
LDFLAGS= 
LIBS= 

# Clean
RM= rm
RMFLAGS= -f


SOURCES  := $(wildcard $(SRC_DIR)/*.c)
INCLUDES := $(wildcard $(INCLUDE_DIR)/*.h)
OBJECTS  := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BINARY   := $(BIN_DIR)/$(TARGET)

all: license $(BINARY)
	@echo "Done."


$(BINARY): $(OBJECTS) $(BIN_DIR)
	$(LD) $(LDFLAGS) -o $@ $< $(LIBS)

%.o: %.c $(INCLUDES)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDE)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)
	
.PHONY: clean
clean:
	$(RM) $(RMFLAGS) $(OBJECTS) $(BINARY)


license:
	cat LICENSE


