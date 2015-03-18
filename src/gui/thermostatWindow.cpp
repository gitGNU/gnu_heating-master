/*
 * thermostatWindow.cpp
 *
 *  Created on: 12.03.2015
 *      Author: nore (D.Rabel)
 *     License: GPLv2 or later
 */
#include <iostream>
#include "thermostatWindow.hpp"

ThermostatWindow::~ThermostatWindow()
{

}

void ThermostatWindow::hideWindow()
{
  cout<<"hallo?"<<endl;
  this->hide();
}

void ThermostatWindow::onShow()
{
  if(thermostat->isUpdating())
  {
      plusButton.set_sensitive(false);
      minusButton.set_sensitive(false);
  }
  cout<<"kuckuck"<<endl;
  progressBar.set_fraction((double)(thermostat->getCurrentValue()-thermostat->getMinValue()) / (double)(thermostat->getMaxValue()-thermostat->getMinValue()));
}

void ThermostatWindow::incProgressBar()
{
//  plusButton.set_sensitive(false);
//  minusButton.set_sensitive(false);

  if( ! thermostat->isUpdating() )
  {
    cout<<"inc"<<endl;
    double tmpFraction = progressBar.get_fraction();
    tmpFraction += 0.1;
    if( tmpFraction > 1 )
    {
      tmpFraction = 1;
    }
    progressBar.set_fraction(tmpFraction);

    thermostat->setCurrentValue((unsigned) ((thermostat->getMaxValue()-thermostat->getMinValue())*tmpFraction + thermostat->getMinValue()), {(void*)this, ThermostatWindow::thermostatCallbackWrapper});
  }
}

void ThermostatWindow::decProgressBar()
{
//  plusButton.set_sensitive(false);
//  minusButton.set_sensitive(false);
  if( ! thermostat->isUpdating() )
  {
    cout<<"dec"<<endl;
    double tmpFraction = progressBar.get_fraction();
    tmpFraction -= 0.1;
    if( tmpFraction < 0 )
    {
      tmpFraction = 0;
    }
    progressBar.set_fraction(tmpFraction);

    thermostat->setCurrentValue((unsigned) ((thermostat->getMaxValue()-thermostat->getMinValue())*tmpFraction + thermostat->getMinValue()), {(void*)this, ThermostatWindow::thermostatCallbackWrapper});
  }
}

void ThermostatWindow::thermostatCallbackWrapper(void* objectPointer)
{
  ThermostatWindow* th_is = (ThermostatWindow*)objectPointer;
  th_is->thermostatCallback();
}

void ThermostatWindow::thermostatCallback()
{
//  plusButton.set_sensitive(true);
//  minusButton.set_sensitive(true);
}

void ThermostatWindow::thermostatStateUpdate(bool updating, bool upToDate, unsigned currentValue)
{
  if( updating || !upToDate )
  {
    plusButton.set_sensitive(false);
    minusButton.set_sensitive(false);
  }
  else
  {
    plusButton.set_sensitive(true);
    minusButton.set_sensitive(true);
  }
  cout<<"auf wiedersehen"<<endl;
  progressBar.set_fraction((double)(currentValue-thermostat->getMinValue()) / (double)(thermostat->getMaxValue()-thermostat->getMinValue()));
  cout<<"oder was"<<endl;
}
