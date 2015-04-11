/*
 * thermostatWindow.hpp
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

#ifndef SRC_GUI_THERMOSTATWINDOW_HPP_
#define SRC_GUI_THERMOSTATWINDOW_HPP_

#include <gtkmm.h>
#include <gtkmm/window.h>
#include "../core/thermostat.hpp"


class ThermostatWindow : public Gtk::Window
{
  /*
   * Window to handle one thermostat, including back button
   * and buttons to adjust the thermostat value
   */
  public:
    /*
     * Constructor
     */
    ThermostatWindow(Thermostat* th) :
      thermostat(th),
      vbox(), minusButton("-"), plusButton("+"), backButton("back"), progressBar(),
      updateThread(0), updateThermostatValueDispatcher(), updateProgressBarDispatcher()
    {
      /*
       * Resize buttons and connect signals
       */
      backButton. set_size_request( 480,136 );
      minusButton.set_size_request( 136,136 );
      plusButton. set_size_request( 136,136 );
      progressBar.set_size_request( 208,136 );

      backButton. signal_clicked().connect( sigc::mem_fun(*this, &ThermostatWindow::hideWindow    ) );
      minusButton.signal_clicked().connect( sigc::mem_fun(*this, &ThermostatWindow::decProgressBar) );
      plusButton. signal_clicked().connect( sigc::mem_fun(*this, &ThermostatWindow::incProgressBar) );

      /*
       * Connect dispatchers to call from worker threads
       */
      updateThermostatValueDispatcher.connect(sigc::mem_fun(*this, &ThermostatWindow::calledFromUpdateThermostatValue));
      updateProgressBarDispatcher.connect(sigc::mem_fun(*this, &ThermostatWindow::calledFromUpdateProgressBar));

      /*
       * Pack and show
       */
      hbox.pack_start(minusButton);
      hbox.pack_start(progressBar);
      hbox.pack_start(plusButton);
      vbox.pack_start(backButton);
      vbox.pack_start(hbox);
      add(vbox);
      vbox.show_all_children(true);
      vbox.show();

      /*
       * Connect show signal
       */
      this->signal_show().connect( sigc::mem_fun(*this, &ThermostatWindow::onShow) );

      /* Set initial progress bar value */
      progressBar.set_fraction(0.5);
      /* Update progress bar from physical thermostat */
      updateProgressBar();

      /* make it fullscreen */
      //set_default_size(400, 422);
      fullscreen();
    }
    virtual ~ThermostatWindow();


  private:
    /*
     * Thermostat
     */
    Thermostat* thermostat;
    /*
     * GTK+ components
     */
    Gtk::VBox vbox;
    Gtk::HBox hbox;
    Gtk::Button minusButton, plusButton, backButton;
    Gtk::ProgressBar progressBar;
    /*
     * Signal handlers
     */
    void hideWindow();
    void onShow();
    void incProgressBar();
    void decProgressBar();
    /*
     * Methods for parallel updating of thermostat or progress bar
     */
    void updateThermostatValue();
    void updateProgressBar();
    void updateThermostatValueThread(unsigned value);
    void updateProgressBarThread();
    void calledFromUpdateThermostatValue();
    void calledFromUpdateProgressBar();
    bool updating;   /* update thread running? */
    /*
     * Working thread and dispatcher
     */
    Glib::Thread* updateThread;
    Glib::Dispatcher updateThermostatValueDispatcher, updateProgressBarDispatcher;
};



#endif /* SRC_GUI_THERMOSTATWINDOW_HPP_ */
