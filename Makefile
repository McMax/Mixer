CC	= g++
LD	= g++

CCFLAGS = -g -O0 `root-config --cflags` -Wall -I./inc -I$(PEV_INC)
LDFLAGS = -g -O0 `root-config --libs` -Wall -L./lib -L$(PEV_LIB)

TOPDIR = .
SRC_DIR = $(TOPDIR)/src
OBJ_DIR = $(TOPDIR)/lib
INC_DIR = $(TOPDIR)/inc

PEV_DIR = $(TOPDIR)/../Particle_Event
PEV_LIB = $(PEV_DIR)/lib
PEV_SRC = $(PEV_DIR)/src
PEV_INC = $(PEV_DIR)/inc

SOURCES := $(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
PEV_OBJECTS = $(PEV_LIB)/Particle.o $(PEV_LIB)/Event.o $(PEV_LIB)/ParticleTree.o $(PEV_LIB)/Dict.o

all: mixer0 splitter

splitter: $(OBJ_DIR)/splitter.o
	$(LD) $(LDFLAGS) $< -o $@ 

mixer0: $(OBJ_DIR)/mixer0.o $(PEV_OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) -c $(CCFLAGS) $< -o $@ 

$(PEV_OBJECTS):
	@echo "No base libs. Create them"
	
clean:
	@rm -rf mixer0 splitter ./lib	
