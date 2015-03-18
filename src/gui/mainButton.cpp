/*
 * mainButton.cpp
 *
 *  Created on: 12.03.2015
 *      Author: nore (D.Rabel)
 *     License: GPLv2 or later
 */

#include "mainButton.hpp"
#include <chrono>
#include <thread>
#include <iostream>

void MainButton::onClicked()
{
  cout<<"aufruf von außen"<<endl;

  window.show();
}


MainButton::~MainButton()
{
}

void MainButton::thermostatStateUpdate( bool updating, bool upToDate, unsigned currentValue )
{
  if( updating || upToDate )
  {
    set_sensitive(true);
  }
  else
  {
    set_sensitive(false);
  }
}
