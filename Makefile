#
# Makefile
#
#   Copyright (c) 2015, David Rabel
#   All rights reserved. 
#
#   Redistribution and use in source and binary forms, with or without modification, 
#   are permitted provided that the following conditions are met:
#
#   1. Redistributions of source code must retain the above copyright notice, 
#      this list of conditions and the following disclaimer.
#   
#   2. Redistributions in binary form must reproduce the above copyright notice, 
#      this list of conditions and the following disclaimer in the documentation and/or 
#      other materials provided with the distribution.
#   
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
#   EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
#   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
#   SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
#   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
#   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
#   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
#   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
#   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
#   DAMAGE. 
#

# Directories
SRC_DIR        = src
OBJ_DIR        = obj
EXE_DIR        = exe
VPATH          = $(SRC_DIR)/*
# Source files
SRC_CORE       = $(SRC_DIR)/core/thermostat.cpp $(SRC_DIR)/core/heatingMaster.cpp $(SRC_DIR)/core/jsonaux.cpp
SRC_EXT        = $(SRC_DIR)/external/json-parser/json.cpp
SRC_CMDLINE    = $(SRC_DIR)/cmdline/main.cpp
SRC_GUI        = $(SRC_DIR)/gui/guiMain.cpp $(SRC_DIR)/gui/mainWindow.cpp $(SRC_DIR)/gui/mainButton.cpp $(SRC_DIR)/gui/thermostatWindow.cpp
SRC_ALL_BUT_EXT= $(SRC_CORE) $(SRC_CMDLINE) $(SRC_GUI)
# Object files
TMP_OBJ_CMDLINE= $(SRC_CORE) $(SRC_EXT) $(SRC_CMDLINE)
OBJ_CMDLINE    = $(TMP_OBJ_CMDLINE:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) 
TMP_OBJ_GUI    = $(SRC_CORE) $(SRC_EXT) $(SRC_GUI)
OBJ_GUI        = $(TMP_OBJ_GUI:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) 
# Dependency files
DEP            = $(SRC_ALL_BUT_EXT:%.cpp=%.dep)
# Compiler flags
CFLAGS         = -Wall -std=c++11
CFLAGS_CORE    = $(CFLAGS) 
CFLAGS_CMDLINE = $(CFLAGS_CORE) -pthread
CFLAGS_GUI     = $(CFLAGS_CORE) `pkg-config --cflags --libs gtkmm-3.0`
# Dependencies file
DEPENDFILE = dependencies

# Targets 

all: distclean dep cmdline gui

install: all
	cp -r .heating-master ~
	sudo cp -r $(EXE_DIR)/* /usr/local/bin/

uninstall:
	rm -rf ~/.heating-master
	sudo rm -f /usr/local/bin/heating-master /usr/local/bin/heating-master-gui 

distclean: 
	rm -rf $(OBJ_DIR)
	rm -rf $(EXE_DIR)
	rm -f $(DEPENDFILE)

dep: $(DEP)
	cat $(DEP) > $(DEPENDFILE)
	rm -f $(SRC_DIR)/*/*.dep

%.dep: %.cpp
	if ! [ -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	if ! [ -d $(@D:$(SRC_DIR)/%=$(OBJ_DIR)/%) ]; then mkdir $(@D:$(SRC_DIR)/%=$(OBJ_DIR)/%); fi
	if ! [ -d $(EXE_DIR) ]; then mkdir $(EXE_DIR); fi
	g++ -std=c++11 -MM $< -MT $(*:$(SRC_DIR)/%=$(OBJ_DIR)/%).o >>$@

-include $(DEPENDFILE)


cmdline: $(OBJ_CMDLINE)
	g++ $(OBJ_CMDLINE) -o $(EXE_DIR)/heating-master $(CFLAGS_CMDLINE)

gui: $(OBJ_GUI)
	g++ $(OBJ_GUI) -o $(EXE_DIR)/heating-master-gui $(CFLAGS_GUI)
	

$(OBJ_DIR)/core/%.o: $(SRC_DIR)/core/%.cpp Makefile
	g++ -c $< -o $@ $(CFLAGS_CORE)

$(OBJ_DIR)/external/json-parser/json.o: $(SRC_DIR)/external/json-parser/json.c $(SRC_DIR)/external/json-parser/json.h
	if ! [ -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	if ! [ -d $(OBJ_DIR)/external ]; then mkdir $(OBJ_DIR)/external; fi
	if ! [ -d $(OBJ_DIR)/external/json-parser ]; then mkdir $(OBJ_DIR)/external/json-parser; fi
	g++ -c $< -o $@ $(CFLAGS_CORE)

$(OBJ_DIR)/cmdline/%.o: $(SRC_DIR)/cmdline/%.cpp Makefile
	g++ -c $< -o $@ $(CFLAGS_CMDLINE)
	
$(OBJ_DIR)/gui/%.o: $(SRC_DIR)/gui/%.cpp Makefile
	g++ -c $< -o $@ $(CFLAGS_GUI)
