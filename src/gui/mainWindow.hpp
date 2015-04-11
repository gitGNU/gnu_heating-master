/*
 * mainWindow.hpp
 *
 *  Copyright (C) 2015, David Rabel
 *  All rights reserved.
 *
 *  This file is part of Heating Master.
 *
 *    Heating Master is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    Heating Master is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Heating Master; see the file COPYING.
 *    If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SRC_GUI_MAINWINDOW_HPP_
#define SRC_GUI_MAINWINDOW_HPP_

#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/viewport.h>
#include <gtkmm/scrolledwindow.h>
#include "../core/heatingMaster.hpp"
#include "../core/thermostat.hpp"


class MainWindow : public Gtk::Window
{
  /*
   * Class for main window
   */
  public:
    /* constructor and destructor */
    MainWindow(HeatingMaster& hm);
    virtual ~MainWindow();

  private:
    /* An instance of heatingMaster class */
    HeatingMaster& heatingMaster;
    /* Some Gtk+ objects */
    Gtk::VBox vbox;
    Gtk::ScrolledWindow scrolledWindow;
};



#endif /* SRC_GUI_MAINWINDOW_HPP_ */
