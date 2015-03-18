/*
 * guiMain.cpp
 *
 *  Created on: 2015-03
 *      Author: D.Rabel
 *     License: See LICENSE.txt in the root folder of this project. //todo lizenz abklären
 */

#include <gtkmm.h>
#include <cstdlib>
#include <iostream>
#include "../gui/guiMain.hpp"  //todo inlcude reihenfolge

#include "mainWindow.hpp"

int main(int argc, char** argv)
{
  Glib::RefPtr<Gtk::Application> app =
    Gtk::Application::create(argc, argv,
      "org.gtkmm.examples.base"); //todo

  HeatingMaster heatingMaster((string)getenv("HOME")+"/.heating-master/config.json");
  //todo warten bis initialisiert

  MainWindow window(heatingMaster);

  return app->run(window);
}



