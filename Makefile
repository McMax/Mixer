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
INCLUDES := inc/Particle.h inc/Event.h inc/ParticleTree.h inc/linkdef.h

all: $(PROGRAM) splitter

splitter: split/splitter.o
	$(LD) $(LDFLAGS) $< -o $@ 

split/splitter.o: split/splitter.cpp
	$(CC) -c $(CCFLAGS) $< -o $@ 

$(PROGRAM): $(OBJECTS) Dict.o
	$(LD) $(LDFLAGS) $^ -o $@ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) -c $(CCFLAGS) $< -o $@ 

Dict.o: Dict.cpp
	$(CC) -c $(CCFLAGS) Dict.cpp -o Dict.o

Dict.cpp: $(INCLUDES)
	@echo "Generating dictionary..."
	@rootcint -f Dict.cpp -c -P -I$(ROOTSYS) -I/usr/local/include $(INCLUDES)

clean:
	@rm -rf $(PROGRAM) ./lib	
	@rm -f splitter split/splitter.o

realclean: clean
	@rm -f Dict.*
