/*
 * thermostatWindow.cpp
 *
 *  Created on: 2015-03
 *      Author: D.Rabel
 *     License: See LICENSE.txt in the root folder of this project.
 */

#include "thermostatWindow.hpp"

/* Destructor */
ThermostatWindow::~ThermostatWindow()
{
}

/* Handler for back button */
void ThermostatWindow::hideWindow()
{
  this->hide();
}

/* Handler for signal show */
void ThermostatWindow::onShow()
{
  /*
   * If there is no update thread running,
   * start one to update current value of
   * progress bar.
   */
  if( !updating )
  {
    updateProgressBar();
  }
}

/* Handler for + button */
void ThermostatWindow::incProgressBar()
{
  /*
   * If there is an update thread already running,
   * the button shouldn't have been clicked. So updating
   * should always be false. If not, do nothing.
   */
  if( ! updating )
  {
    /*
     * Compute new progress bar value and call method
     * to update new value in thermostat.
     */
    double tmpFraction = progressBar.get_fraction();
    tmpFraction += 0.1;
    if( tmpFraction > 1 )
    {
      tmpFraction = 1;
    }
    progressBar.set_fraction(tmpFraction);

    updateThermostatValue();
  }
}

/* Handler for - button */
void ThermostatWindow::decProgressBar()
{
  /*
   * If there is an update thread already running,
   * the button shouldn't have been clicked. So updating
   * should always be false. If not, do nothing.
   */
  if( ! updating )
  {
    /*
     * Compute new progress bar value and call method
     * to update new value in thermostat.
     */
    double tmpFraction = progressBar.get_fraction();
    tmpFraction -= 0.1;
    if( tmpFraction < 0 )
    {
      tmpFraction = 0;
    }
    progressBar.set_fraction(tmpFraction);

    updateThermostatValue();
  }
}

/* Called to update thermostat from progress bar */
void ThermostatWindow::updateThermostatValue()
{
  /* We are going to start an update thread */
  updating = true;
  /* + and - button are disabled while an update thread is running */
  plusButton.set_sensitive(false);
  minusButton.set_sensitive(false);
  /* compute new thermostat value */
  unsigned tmpValue =(unsigned) ((thermostat->getMaxValue()-thermostat->getMinValue())*progressBar.get_fraction() + thermostat->getMinValue());

  /* join update thread if neccessary */
  if( updateThread && updateThread->joinable() )
  {
    updateThread->join();
  }
  /* start update thread */
  updateThread  = Glib::Thread::create( sigc::bind(sigc::mem_fun(*this, &ThermostatWindow::updateThermostatValueThread), tmpValue) );
}

/* Thread for updating thermostat */
void ThermostatWindow::updateThermostatValueThread(unsigned value)
{
  /*
   * set value and initiate update of
   * physical thermostat from local value
   */
  thermostat->setCurrentValue(value);
  thermostat->updateSlaveFromLocalValue();
  /*
   * when done, call the dispatcher because this thread is not allowed
   * to access GTK+ objects generated at the main thread
   */
  updateThermostatValueDispatcher.emit();
}

/* Handler for dispatcher */
void ThermostatWindow::calledFromUpdateThermostatValue()
{
  /* update progress bar again to see if updating was successfull */
  updateProgressBar();
}

/* Called to update progress bar from thermostat */
void ThermostatWindow::updateProgressBar()
{
  /* We are going to start an update thread */
  updating = true;
  /* + and - button are disabled while an update thread is running */
  plusButton.set_sensitive(false);
  minusButton.set_sensitive(false);
  /* join update thread if neccessary */
  if( updateThread && updateThread->joinable() )
  {
    updateThread->join();
  }
  /* start update thread */
  updateThread  = Glib::Thread::create( sigc::mem_fun(*this, &ThermostatWindow::updateProgressBarThread) );
}

/* Thread for updating progress bar */
void ThermostatWindow::updateProgressBarThread()
{
  /* Initiate update of local value from physical thermostat */
  thermostat->updateLocalValueFromSlave();
  /*
   * when done, call the dispatcher because this thread is not allowed
   * to access GTK+ objects generated at the main thread
   */
  updateProgressBarDispatcher.emit();
}

/* Handler for dispatcher */
void ThermostatWindow::calledFromUpdateProgressBar()
{
  /* Compute new progress bar value from thermostat value */
  progressBar.set_fraction((double)(thermostat->getCurrentValue()-thermostat->getMinValue()) / (double)(thermostat->getMaxValue()-thermostat->getMinValue()));
  /* we're done with updating, a new update thread could be started */
  updating = false;
  /* If updating was successfull, + and - button are activated again */
  if( thermostat->isUpToDate() )
  {
    plusButton.set_sensitive(true);
    minusButton.set_sensitive(true);
  }
}
