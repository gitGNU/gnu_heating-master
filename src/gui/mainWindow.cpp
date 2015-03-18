/*
 * mainWindow.cpp
 *
 *  Created on: 2015-03
 *      Author: D.Rabel
 *     License: See LICENSE.txt in the root folder of this project. //todo lizenz abklären
 */

#include "mainWindow.hpp"
#include <gtkmm.h>
#include <gdkmm.h>
#include <cstdlib>
#include <gtkmm/scrollbar.h>
#include <gdkmm/display.h>
#include <gtkmm/stylecontext.h>
#include <gtkmm/scrolledwindow.h>
#include "mainButton.hpp"

MainWindow::MainWindow(HeatingMaster& hm) : heatingMaster(hm), vbox(), scrolledWindow()
{
  Gtk::Button* button;

  for( unsigned i=0; i<heatingMaster.getNumberOfThermostats(); i++ )
  {
    button = Gtk::manage(new MainButton(heatingMaster.getThermostat(i)->getName(), heatingMaster.getThermostat(i))); //todo delete
    vbox.pack_start(*button);
    button->set_size_request( 400,136 );
    button->show();
  }

  scrolledWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS);
  scrolledWindow.add(vbox);
  vbox.show();
  this->add(scrolledWindow);
  scrolledWindow.show();


  //fullscreen();

  Glib::RefPtr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
  Glib::RefPtr<Gdk::Display> display = Gdk::Display::get_default();
  Glib::RefPtr<Gdk::Screen> screen = display->get_default_screen();
  cssProvider->load_from_path((string)getenv("HOME")+"/.heating-master/style.css");
  Gtk::StyleContext::add_provider_for_screen(screen, cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);

  set_default_size(400, 272);

}

MainWindow::~MainWindow()
{
  //todo delete buttons
}
