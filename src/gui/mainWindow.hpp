/*
 * mainWindow.hpp
 *
 *  Copyright (C) 2015, David Rabel
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation and/or
 *     other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 *  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 *  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 *  DAMAGE.
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
