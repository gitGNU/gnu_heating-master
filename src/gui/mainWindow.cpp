/*
 * mainWindow.cpp
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
    cssProvider->load_from_path((string)getenv("HOME")+"/.heating-master/style.css");
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
