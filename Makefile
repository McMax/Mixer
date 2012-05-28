CC	= g++
LD	= g++

CCFLAGS = -g -O0 `root-config --cflags` -Wall -I./inc
LDFLAGS = -g -O0 `root-config --libs` -Wall -L./lib

TOPDIR = .
SRC_DIR = $(TOPDIR)/src
OBJ_DIR = $(TOPDIR)/lib
INC_DIR = $(TOPDIR)/inc

PROGRAM = mixer0

SOURCES := $(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
INCLUDES := $(shell find $(INC_DIR) -type f -name "*.h")

all: $(PROGRAM) splitter

splitter: split/splitter.o
	$(LD) $(LDFLAGS) $< -o $@ 

split/splitter.o: split/splitter.cpp
	$(CC) -c $(CCFLAGS) $< -o $@ 

$(PROGRAM): $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) -c $(CCFLAGS) $< -o $@ 

Dict.cpp: $(INCLUDES)
	@echo "Generating dictionary..."
	@rootcint -f src/Dict.cpp -c -P -I$(ROOTSYS) -I/usr/local/include $(INCLUDES)

clean:
	@rm $(PROGRAM) -r ./lib	
	@rm splitter split/splitter.o

