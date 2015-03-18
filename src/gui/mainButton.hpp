/*
 * mainButton.hpp
 *
 *  Created on: 12.03.2015
 *      Author: nore (D.Rabel) todo
 *     License: GPLv2 or later
 */

#ifndef SRC_GUI_MAINBUTTON_HPP_
#define SRC_GUI_MAINBUTTON_HPP_

#include <gtkmm.h>
#include "../core/thermostat.hpp"
#include "thermostatWindow.hpp"


class MainButton : public Gtk::Button, public ThermostatObserver
{
  public:
    MainButton(const Glib::ustring & label, Thermostat* th) : Gtk::Button(label), thermostat(th), window(th)
    {
      //window.hide();
      signal_clicked().connect( sigc::mem_fun(*this, &MainButton::onClicked) );
      th->attachObserver(this);
    }
    virtual ~MainButton();

    void onClicked();
    void thermostatStateUpdate(bool updating, bool upToDate, unsigned currentValue);

  private:
    Thermostat*       thermostat;
    ThermostatWindow  window;
};



#endif /* SRC_GUI_MAINBUTTON_HPP_ */
