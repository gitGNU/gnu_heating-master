/*
 * thermostatWindow.hpp
 *
 *  Created on: 12.03.2015
 *      Author: nore (D.Rabel)
 *     License: GPLv2 or later
 */

#ifndef SRC_GUI_THERMOSTATWINDOW_HPP_
#define SRC_GUI_THERMOSTATWINDOW_HPP_

#include <gtkmm.h>
#include <gtkmm/window.h>
#include <iostream>
#include "../core/thermostat.hpp"



class ThermostatWindow : public Gtk::Window, public ThermostatObserver
{
  public:
    ThermostatWindow(Thermostat* th) : thermostat(th), vbox(), minusButton("-"), plusButton("+"), backButton("back"), progressBar()
    {
      th->attachObserver(this);

      set_default_size(400, 422);

      backButton. set_size_request( 400,136 );
      minusButton.set_size_request( 136,136 );
      plusButton. set_size_request( 136,136 );
      progressBar.set_size_request( 136,136 );

      backButton. signal_clicked().connect( sigc::mem_fun(*this, &ThermostatWindow::hideWindow    ) );
      minusButton.signal_clicked().connect( sigc::mem_fun(*this, &ThermostatWindow::decProgressBar) );
      plusButton. signal_clicked().connect( sigc::mem_fun(*this, &ThermostatWindow::incProgressBar) );


      hbox.pack_start(minusButton);
      hbox.pack_start(progressBar);
      hbox.pack_start(plusButton);
      vbox.pack_start(backButton);
      vbox.pack_start(hbox);
      add(vbox);
      vbox.show_all_children(true);
      vbox.show();

      cout<<"hallo"<<endl;
      cout<<(double)(th->getCurrentValue()-th->getMinValue()) / (double)(th->getMaxValue()-th->getMinValue())<<endl;
      progressBar.set_fraction((double)(th->getCurrentValue()-th->getMinValue()) / (double)(th->getMaxValue()-th->getMinValue()));
      //fullscreen();
      this->signal_show().connect( sigc::mem_fun(*this, &ThermostatWindow::onShow) );
    } // don't do this at home todo
    virtual ~ThermostatWindow();

    static void thermostatCallbackWrapper(void* objectPointer);
    void thermostatCallback();
    void thermostatStateUpdate(bool updating, bool upToDate, unsigned currentValue);

  private:
    Thermostat* thermostat;
    Gtk::VBox vbox;
    Gtk::HBox hbox;
    Gtk::Button minusButton, plusButton, backButton;
    Gtk::ProgressBar progressBar;
    void hideWindow();
    void onShow();
    void incProgressBar();
    void decProgressBar();
};



#endif /* SRC_GUI_THERMOSTATWINDOW_HPP_ */
