/*
 * mainWindow.hpp
 *
 *  Created on: 2015-03
 *      Author: D.Rabel
 *     License: See LICENSE.txt in the root folder of this project. //todo lizenz abklären
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
  public:
    MainWindow(HeatingMaster& hm);
    virtual ~MainWindow();

  private:
    HeatingMaster& heatingMaster;
    // todo buttons
    Gtk::VBox vbox;
    Gtk::ScrolledWindow scrolledWindow;
    //Gtk::Viewport vp;
};



#endif /* SRC_GUI_MAINWINDOW_HPP_ */
