#
# Makefile
#
#   Copyright (c) 2015, David Rabel
#   All rights reserved. 
#
#   This file is part of Heating Master.
#  
#     Heating Master is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 2 of the License, or
#     (at your option) any later version.
#  
#     Heating Master is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#     GNU General Public License for more details.
#  
#     You should have received a copy of the GNU General Public License
#     along with Heating Master; see the file COPYING. 
#     If not, see <http://www.gnu.org/licenses/>.
#  
#

# Version
VERSION_FILE   = VERSION
VERSION        = \"`cat $(VERSION_FILE) | awk '{printf $$1 }'`\"
# Directories
SRC_DIR        = src
OBJ_DIR        = obj
EXE_DIR        = exe
CONF_DIR       = config
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
CFLAGS_CORE    = $(CFLAGS) -DVERSION=$(VERSION)
CFLAGS_CMDLINE = $(CFLAGS_CORE) -pthread
CFLAGS_GUI     = $(CFLAGS_CORE) `pkg-config --cflags --libs gtkmm-3.0`
# Dependencies file
DEPENDFILE = dependencies

# Targets

.PHONY: all install uninstall distclean dir dep cmdline gui

all: dep cmdline gui

install: all
	install -d $(DESTDIR)/usr/local/bin/
	sudo install $(EXE_DIR)/* $(DESTDIR)/usr/local/bin/
	if ! [ `install -d $(DESTDIR)/etc/heating-master/ 2>/dev/null` ]; then sudo install -d $(DESTDIR)/etc/heating-master/; fi;
	sudo install -o $(USER) -g `id -gn` -m 644 $(CONF_DIR)/* $(DESTDIR)/etc/heating-master

uninstall:
	sudo rm -rf $(DESTDIR)/etc/heating-master
	sudo rm -f $(DESTDIR)/usr/local/bin/heating-master $(DESTDIR)/usr/local/bin/heating-master-gui
	rm -rf $(DESTDIR)

distclean:
	rm -rf $(OBJ_DIR)
	rm -rf $(EXE_DIR)
	rm -f $(DEPENDFILE)

dir:
	if ! [ -d $(EXE_DIR) ]; then mkdir $(EXE_DIR); fi
	if ! [ -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	cd $(SRC_DIR); find -type d -exec mkdir -p "../$(OBJ_DIR)/{}" \;

dep: dir $(DEP)
	cat $(DEP) > $(DEPENDFILE)
	rm -f $(SRC_DIR)/*/*.dep

%.dep: %.cpp Makefile
	g++ -std=c++11 -MM $< -MT $(*:$(SRC_DIR)/%=$(OBJ_DIR)/%).o >>$@

-include $(DEPENDFILE)


cmdline: $(OBJ_CMDLINE)
	g++ $(OBJ_CMDLINE) -o $(EXE_DIR)/heating-master $(CFLAGS_CMDLINE)

gui: $(OBJ_GUI)
	g++ $(OBJ_GUI) -o $(EXE_DIR)/heating-master-gui $(CFLAGS_GUI)
	

$(OBJ_DIR)/core/%.o: $(SRC_DIR)/core/%.cpp Makefile $(VERSION_FILE)
	g++ -c $< -o $@ $(CFLAGS_CORE)

$(OBJ_DIR)/external/json-parser/json.o: $(SRC_DIR)/external/json-parser/json.c $(SRC_DIR)/external/json-parser/json.h Makefile
	g++ -c $< -o $@ $(CFLAGS_CORE)

$(OBJ_DIR)/cmdline/%.o: $(SRC_DIR)/cmdline/%.cpp Makefile $(VERSION_FILE)
	g++ -c $< -o $@ $(CFLAGS_CMDLINE)
	
$(OBJ_DIR)/gui/%.o: $(SRC_DIR)/gui/%.cpp Makefile $(VERSION_FILE)
	g++ -c $< -o $@ $(CFLAGS_GUI)
