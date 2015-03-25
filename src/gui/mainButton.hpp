/*
 * mainButton.hpp
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

#ifndef SRC_GUI_MAINBUTTON_HPP_
#define SRC_GUI_MAINBUTTON_HPP_

#include <gtkmm.h>
#include "../core/thermostat.hpp"
#include "thermostatWindow.hpp"


class MainButton : public Gtk::Button
{
  /*
   * Button for the main window
   */
  public:
    /*
     * Constructor
     */
    MainButton(const Glib::ustring & label, Thermostat* th) : Gtk::Button(label), window(th)
    {
      /* connect method onClicked() to clicked signal */
      signal_clicked().connect( sigc::mem_fun(*this, &MainButton::onClicked) );
    }

    /* virtual destructor */
    virtual ~MainButton();

    /* onClicked() is called when button is clicked */
    void onClicked();

  private:
    /* ThermostatWindow is shown everytime the button is clicked */
    ThermostatWindow  window;
};



#endif /* SRC_GUI_MAINBUTTON_HPP_ */
