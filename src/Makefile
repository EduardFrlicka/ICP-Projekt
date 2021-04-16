#AUTHOR : Eduard Frlička


# =========================================================================
# Names and Directories

# Flags for compiling
CFLAGS=-std=c++17 -Wall -Wextra -pedantic
CFLAGS+=$(shell pkg-config --cflags Qt5Widgets)

LFLAGS=-std=c++11 -Wall -Wextra -pedantic
LFLAGS+=$(shell pkg-config --libs Qt5Widgets)

# Name of target binary file
BINARY_NAME=binary

# Name / Path to bin folder (for storing binary files)
BIN=../bin

# Name / Path to folder, where .o files will be stored
OBJ=../obj

# Name / Path to folder, where source files are (.h .c .cpp ...)
SRC=.

# Compiler
CC=g++
MOC=moc-qt5

# Suffix of files to compile (E.x.: c, cpp, c++)
SUFFIX=cpp

# rm command
RM=rm -f


# =========================================================================
# initializing global variables
BINARY_PATH=$(BIN)/$(BINARY_NAME)

SRC_DIRECTORIES=$(shell find $(SRC) -type d)

vpath %.$(SUFFIX) $(SRC_DIRECTORIES)
vpath %.h $(SRC_DIRECTORIES)

# Adding -I 
IFLAGS := $(foreach DIR, $(SRC_DIRECTORIES),-I $(DIR))

SOURCES 	:= $(foreach DIR,$(SRC_DIRECTORIES),$(notdir $(wildcard $(DIR)/*.$(SUFFIX))))
OBJECTS 	:= $(patsubst %.$(SUFFIX),$(OBJ)/%.o,$(SOURCES))
HEADERS 	:= $(foreach DIR,$(SRC_DIRECTORIES),$(notdir $(wildcard $(DIR)/*.h)))
OBJECTS 	+= $(patsubst %.h, $(OBJ)/%.h.o,$(HEADERS))


# =========================================================================
# Functions

# checks if dir exists, then creates whole path
check_dir = @if [ ! -d $1 ]; then mkdir -p $1; echo "Created directory: $1"; fi 


# =========================================================================
# Targets
.PHONY:  all run clean cleanall source
.SECONDARY:

# =========================================================================
# Rules
all: $(BINARY_PATH) 

run: $(BINARY_PATH) 
	./$(BINARY_PATH)

$(BINARY_PATH) : $(OBJECTS)
	$(call check_dir,$(@D))
	$(CC)  $(OBJECTS) -o $@ $(LFLAGS)

$(OBJ)/%.h.cpp: %.h
	$(call check_dir,$(@D))
	$(MOC) $< -o $@

$(OBJ)/%.h.o: $(OBJ)/%.h.cpp
	$(call check_dir,$(@D))
	$(CC)  $< -c -o $@ $(CFLAGS)


$(OBJ)/%.o: %.$(SUFFIX) $(HEADERS) 
	$(call check_dir,$(@D))
	$(CC)  $< -c -o $@ $(CFLAGS) $(IFLAGS)


source:
	@echo "Source_Folders   : " $(SRC_DIRECTORIES)
	@echo "Sources          : " $(SOURCES)
	@echo "Headers          : " $(HEADERS)
	@echo "Objects          : " $(OBJECTS)


# =========================================================================
# Cleaning rules
clean:
	$(RM) -rf $(OBJ) $(BIN)
