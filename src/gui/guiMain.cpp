/*
 * guiMain.cpp
 *
 *  Created on: 2015-03
 *      Author: D.Rabel
 *     License: See LICENSE.txt in the root folder of this project.
 */

#include <gtkmm.h>
#include <string>
#include "mainWindow.hpp"

using namespace std;

int main(int argc, char** argv)
{
  /*
   * Build GTK+ Application
   */
  Glib::RefPtr<Gtk::Application> app =
    Gtk::Application::create(argc, argv,
      "com.noresoft.heating-master-gui");

  /* Initialize Glib threads */
  if(!Glib::thread_supported()) Glib::thread_init();

  /* Initialize heating master */
  HeatingMaster heatingMaster((string)getenv("HOME")+"/.heating-master/config.json");

  /* Initialize main window */
  MainWindow window(heatingMaster);

  /* run */
  return app->run(window);
}



