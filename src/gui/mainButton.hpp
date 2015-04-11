/*
 * mainButton.hpp
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
