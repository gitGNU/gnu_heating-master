/*
 * mainWindow.cpp
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
 *    along with Heating Master. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gdkmm.h>
#include <cstdlib>
#include <gtkmm/scrollbar.h>
#include <gdkmm/display.h>
#include <gtkmm/stylecontext.h>
#include "mainButton.hpp"
#include "mainWindow.hpp"

using namespace std;

/*
 * Constructor
 */
MainWindow::MainWindow(HeatingMaster& hm) : heatingMaster(hm), vbox(), scrolledWindow()
{
  /*
   * Add one button for each thermostat
   */
  Gtk::Button* button;
  for( unsigned i=0; i<heatingMaster.getNumberOfThermostats(); i++ )
  {
    button = Gtk::manage(new MainButton(heatingMaster.getThermostat(i)->getName(), heatingMaster.getThermostat(i)));
    vbox.pack_start(*button);
    button->set_size_request( 380,136 );
    button->show();
  }

  /* scroll only vertical */
  scrolledWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS);
  /* pack and show everything */
  scrolledWindow.add(vbox);
  vbox.show();
  this->add(scrolledWindow);
  scrolledWindow.show();

  /* Get style from CSS file */
  try
  {
    Glib::RefPtr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
    Glib::RefPtr<Gdk::Display> display = Gdk::Display::get_default();
    Glib::RefPtr<Gdk::Screen> screen = display->get_default_screen();
    cssProvider->load_from_path("/etc/heating-master/style.css");
    Gtk::StyleContext::add_provider_for_screen(screen, cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  }
  catch(...)
  {

  }

  /* Make it fullscreen */
  fullscreen();
  //set_default_size(400, 272);
}

/*
 * Destructor
 */
MainWindow::~MainWindow()
{
  /* nop */
}
