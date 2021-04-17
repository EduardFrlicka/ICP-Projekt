#AUTHOR : Eduard Frlička


# =========================================================================
# Names and Directories

# Flags for compiling
CFLAGS=-std=c++17 -Wall -Wextra -pedantic -fPIC
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
UIC=uic-qt5

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
vpath %.ui $(SRC_DIRECTORIES)

# Adding -I 
IFLAGS := $(foreach DIR, $(SRC_DIRECTORIES),-I $(DIR)) -I$(OBJ)

SOURCES 	:= $(foreach DIR,$(SRC_DIRECTORIES),$(notdir $(wildcard $(DIR)/*.$(SUFFIX))))
OBJECTS 	:= $(patsubst %.$(SUFFIX),$(OBJ)/%.o,$(SOURCES))
HEADERS 	:= $(foreach DIR,$(SRC_DIRECTORIES),$(notdir $(wildcard $(DIR)/*.h)))
OBJECTS 	+= $(patsubst %.h, $(OBJ)/%.h.o,$(HEADERS))
UI  		:= $(foreach DIR,$(SRC_DIRECTORIES),$(notdir $(wildcard $(DIR)/*.ui)))
UI_H		:= $(patsubst %.ui, $(OBJ)/ui_%.h,$(UI))

# =========================================================================
# Functions

# checks if dir exists, then creates whole path


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
	@mkdir -p $(@D)
	$(CC)  $(OBJECTS) -o $@ $(LFLAGS)

$(OBJ)/%.h.cpp: %.h
	@mkdir -p $(@D)
	$(MOC) $< -o $@

$(OBJ)/%.h.o: $(OBJ)/%.h.cpp
	@mkdir -p $(@D)
	$(CC)  $< -c -o $@ $(CFLAGS) -I$(OBJ)

$(OBJ)/ui_%.h: %.ui
	@mkdir -p $(@D)
	$(UIC) $< -o $@

$(OBJ)/%.o: %.$(SUFFIX) $(HEADERS) $(UI_H)
	@mkdir -p $(@D)
	$(CC)  $< -c -o $@ $(CFLAGS) $(IFLAGS)


source:
	@echo "Source_Folders   : " $(SRC_DIRECTORIES)
	@echo "Sources          : " $(SOURCES)
	@echo "Headers          : " $(HEADERS)
	@echo "UI               : " $(UI)
#	@echo "Objects          : " $(OBJECTS)


# =========================================================================
# Cleaning rules
clean:
	$(RM) -rf $(OBJ) $(BIN)
