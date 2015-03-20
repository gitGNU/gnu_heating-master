/*
 * mainButton.hpp
 *
 *  Created on: 2015-03
 *      Author: D.Rabel
 *     License: See LICENSE.txt in the root folder of this project.
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
