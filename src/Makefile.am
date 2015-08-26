AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -DCONF_PATH=\"$(sysconfdir)/heating-master\"

bin_PROGRAMS = heating-master heating-master-gui

HEATING_MASTER_CORE_SRC = core/thermostat.cpp core/thermostat.hpp core/heatingMaster.cpp core/heatingMaster.hpp core/jsonaux.cpp core/jsonaux.hpp
HEATING_MASTER_JSON_SRC = external/json-parser/json.c external/json-parser/json.h
HEATING_MASTER_CMDLINE_SRC = cmdline/main.cpp
HEATING_MASTER_GUI_SRC = gui/guiMain.cpp gui/mainButton.cpp gui/mainButton.hpp gui/mainWindow.cpp gui/mainWindow.hpp gui/thermostatWindow.cpp gui/thermostatWindow.hpp

heating_master_SOURCES = $(HEATING_MASTER_CORE_SRC) $(HEATING_MASTER_JSON_SRC) $(HEATING_MASTER_CMDLINE_SRC)
heating_master_gui_SOURCES = $(HEATING_MASTER_CORE_SRC) $(HEATING_MASTER_JSON_SRC) $(HEATING_MASTER_GUI_SRC)

heating_master_CPPFLAGS = -pthread $(AM_CPPFLAGS)
heating_master_gui_CPPFLAGS = $(GTKMM_CFLAGS) $(AM_CPPFLAGS)
heating_master_LDFLAGS = -pthread
heating_master_gui_LDADD = $(GTKMM_LIBS)
