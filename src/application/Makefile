#AUTHOR : Eduard Frlička


# =========================================================================
# Names and Directories


# Flags for compiling
CFLAGS:=-std=c++17 -Wall -Wextra -pedantic -fPIC #-fpermissive 
CFLAGS+=$(patsubst -I%,-isystem %,$(shell pkg-config --cflags Qt5Widgets))

LFLAGS:=-lpaho-mqtt3c -lpaho-mqtt3a -lpaho-mqttpp3 #-lGL -lpthread 
LFLAGS+=$(shell pkg-config --libs Qt5Widgets)

# Name of target binary file
BINARY_NAME:=app

# Name / Path to bin folder (for storing binary files)
BIN:=../../bin

# Name / Path to folder, where .o files will be stored
OBJ:=../../obj/application

# Name / Path to folder, where source files are (.h .c .cpp ...)
SRC:=.

# Compiler
CXX:=g++

# Suffix of files to compile (E.x.: c, cpp, c++)
SUFFIX=cpp

# rm command
RM=rm -f


# =========================================================================
# initializing global variables

QMAKE:=$(shell (qmake-qt5 -help 1>/dev/null 2>/dev/null  && echo "qmake-qt5") ||  (qmake -help 1>/dev/null 2>/dev/null && echo "qmake"))
QT_BIN:=$(shell $(QMAKE) -query QT_HOST_BINS | sed -E 's/\/*$$//')

MOC:=$(QT_BIN)/moc
UIC:=$(QT_BIN)/uic
RCC:=$(QT_BIN)/rcc

BINARY_PATH=$(BIN)/$(BINARY_NAME)

SRC_DIRECTORIES=$(shell find $(SRC) -type d)

vpath %.$(SUFFIX) $(SRC_DIRECTORIES) $(OBJ)
vpath %.h $(SRC_DIRECTORIES)
vpath %.ui $(SRC_DIRECTORIES)
vpath %.qrc $(SRC_DIRECTORIES)

# Adding -I 
IFLAGS := $(foreach DIR, $(SRC_DIRECTORIES),-I $(DIR)) -I$(OBJ)

SOURCES 	:= $(foreach DIR,$(SRC_DIRECTORIES),$(notdir $(wildcard $(DIR)/*.$(SUFFIX))))
OBJECTS 	:= $(patsubst %.$(SUFFIX),$(OBJ)/%.o,$(SOURCES))
HEADERS 	:= $(foreach DIR,$(SRC_DIRECTORIES),$(notdir $(wildcard $(DIR)/*.h)))
OBJECTS 	+= $(patsubst %.h, $(OBJ)/%.h.o,$(HEADERS))
UI  		:= $(foreach DIR,$(SRC_DIRECTORIES),$(notdir $(wildcard $(DIR)/*.ui)))
UI_H		:= $(patsubst %.ui, $(OBJ)/ui_%.h,$(UI))
QRC 		:= $(foreach DIR,$(SRC_DIRECTORIES),$(notdir $(wildcard $(DIR)/*.qrc)))
OBJECTS 	+= $(patsubst %.qrc, $(OBJ)/qrc_%.o,$(QRC))


# =========================================================================
# Targets
.PHONY:  all run clean cleanall source ui 
.SECONDARY:


# =========================================================================
# Rules
all: $(BINARY_PATH) 

ui: $(UI_H)

run: $(BINARY_PATH) 
	./$(BINARY_PATH)

$(BINARY_PATH) : $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX)  $(OBJECTS) -o $@ $(LFLAGS)

$(OBJ)/%.h.cpp: %.h
	@mkdir -p $(@D)
	$(MOC) $< -o $@

$(OBJ)/%.h.o: $(OBJ)/%.h.cpp
	@mkdir -p $(@D)
	$(CXX)  $< -c -o $@ $(CFLAGS) -I$(OBJ)

$(OBJ)/ui_%.h: %.ui
	@mkdir -p $(@D)
	$(UIC) $< -o $@

$(OBJ)/qrc_%.cpp: %.qrc
	@mkdir -p $(@D)
	$(RCC) $< -o $@

$(OBJ)/%.o: %.$(SUFFIX) $(HEADERS) $(UI_H)
	@mkdir -p $(@D)
	$(CXX)  $< -c -o $@ $(CFLAGS) $(IFLAGS)

source: ui
	@echo "Source_Folders   : " $(SRC_DIRECTORIES)
	@echo "Sources          : " $(SOURCES)
	@echo "Headers          : " $(HEADERS)
	@echo "UI               : " $(UI)
	@echo "QRC              : " $(QRC)
	@echo "Objects          : " $(OBJECTS) 
	@echo "UI H             : " $(UI_H)
	@echo "QMAKE            : " $(QMAKE)

# =========================================================================
# Cleaning rules
clean:
	$(RM) -rf $(OBJ) $(BIN)
