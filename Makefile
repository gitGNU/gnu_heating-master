# todo copyright...
# todo -g entfernen
#
# Directories
SRC_DIR        = src
OBJ_DIR        = obj
EXE_DIR        = exe
VPATH          = $(SRC_DIR)/*
# Source files
SRC_CORE       = $(SRC_DIR)/core/thermostat.cpp $(SRC_DIR)/core/heatingMaster.cpp $(SRC_DIR)/core/jsonaux.cpp
SRC_EXT        = $(SRC_DIR)/external/json.cpp
SRC_CMDLINE    = $(SRC_DIR)/cmdline/main.cpp
SRC_GUI        = $(SRC_DIR)/gui/guiMain.cpp $(SRC_DIR)/gui/mainWindow.cpp $(SRC_DIR)/gui/mainButton.cpp $(SRC_DIR)/gui/thermostatWindow.cpp
SRC_ALL        = $(SRC_CORE) $(SRC_EXT) $(SRC_CMDLINE) $(SRC_GUI)
# Object files
TMP_OBJ_CMDLINE= $(SRC_CORE) $(SRC_EXT) $(SRC_CMDLINE)
OBJ_CMDLINE    = $(TMP_OBJ_CMDLINE:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) 
TMP_OBJ_GUI    = $(SRC_CORE) $(SRC_EXT) $(SRC_GUI)
OBJ_GUI        = $(TMP_OBJ_GUI:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) 
# Dependency files
DEP            = $(SRC_ALL:%.cpp=%.dep)
# Compiler flags
CFLAGS         = -Wall -std=c++11 -g
CFLAGS_CORE    = $(CFLAGS) -pthread
CFLAGS_CMDLINE = $(CFLAGS_CORE)
CFLAGS_GUI     = $(CFLAGS_CORE) `pkg-config --cflags --libs gtkmm-3.0`

DEPENDFILE = dependencies

all: distclean dep cmdline gui

distclean: 
	rm -rf $(OBJ_DIR)/*
	rm -f $(EXE_DIR)/*
	rm -f $(DEPENDFILE)

dep: $(DEP)
	cat $(DEP) > $(DEPENDFILE)
	rm -f $(SRC_DIR)/*/*.dep

%.dep: %.cpp
	if ! [ -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	if ! [ -d $(@D:$(SRC_DIR)/%=$(OBJ_DIR)/%) ]; then mkdir $(@D:$(SRC_DIR)/%=$(OBJ_DIR)/%); fi
	g++ -std=c++11 -MM $< -MT $(*:$(SRC_DIR)/%=$(OBJ_DIR)/%).o >>$@

-include $(DEPENDFILE)

$(OBJ_DIR)/core/%.o: $(SRC_DIR)/core/%.cpp Makefile
	g++ -c $< -o $@ $(CFLAGS_CORE)

$(OBJ_DIR)/external/%.o: $(SRC_DIR)/external/%.cpp Makefile
	g++ -c $< -o $@ $(CFLAGS_CORE)
	
$(OBJ_DIR)/cmdline/%.o: $(SRC_DIR)/cmdline/%.cpp Makefile
	g++ -c $< -o $@ $(CFLAGS_CMDLINE)
	
$(OBJ_DIR)/gui/%.o: $(SRC_DIR)/gui/%.cpp Makefile
	g++ -c $< -o $@ $(CFLAGS_GUI)

cmdline: $(OBJ_CMDLINE)
	g++ $(OBJ_CMDLINE) -o $(EXE_DIR)/heating-master $(CFLAGS_CMDLINE)

gui: $(OBJ_GUI)
	g++ $(OBJ_GUI) -o $(EXE_DIR)/heating-master-gui $(CFLAGS_GUI)
#	
#distclean: 
#	rm -f $(OBJ_DIR)/*
#	rm -f $(EXE_DIR)/*
#
#$(OBJ_DIR)/%.o
#	if ! [ -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
#
#
#obj/thermostat.o: Makefile src/core/thermostat.cpp
#	g++ -c $(SRC_DIR)/core/thermostat.cpp -o obj/thermostat.o $(CFLAGS_CORE)
#	echo $<
#  
#heatingMaster.o: Makefile
#	g++ -c $(SRC_DIR)/core/heatingMaster.cpp -o $@ $(CFLAGS_CORE)
#  
#jsonaux.o: Makefile
#	g++ -c $(SRC_DIR)/core/jsonaux.cpp -o $@ $(CFLAGS_CORE)
#	
#	
#json.o: Makefile
#	g++ -c $(SRC_DIR)/external/json.c -o $@ $(CFLAGS)
#	
#	
#main.o: Makefile
#	g++ -c $(SRC_DIR)/cmdline/main.cpp -o $@ $(CFLAGS_CMDLINE)
#  
#  
#guiMain.o: Makefile
#	g++ -c $(SRC_DIR)/gui/guiMain.cpp -o $@ $(CFLAGS_GUI)
#	
#mainWindow.o: Makefile
#	g++ -c $(SRC_DIR)/gui/mainWindow.cpp -o $@ $(CFLAGS_GUI)
#	
#mainButton.o: Makefile
#	g++ -c $(SRC_DIR)/gui/mainButton.cpp -o $@ $(CFLAGS_GUI)
#	
#thermostatWindow.o: Makefile
#	g++ -c $(SRC_DIR)/gui/thermostatWindow.cpp -o $@ $(CFLAGS_GUI)
#	
#	