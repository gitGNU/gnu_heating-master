/*
 * thermostat.cpp
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

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <string>
#include "thermostat.hpp"
#include "jsonaux.hpp"

using namespace std;

/*
 * Sets current value but doesn't transmit it to
 * physical thermostat over the network.
 *
 * If the value is out of bounds, it will be adjusted.
 *
 * Return value gives if setting was successfull (true)
 * or the value had to be adjusted (false).
 */
bool Thermostat::setCurrentValue( unsigned value )
{
  bool success = true;
  if( value<minValue )
  {
    success = false;
    value = minValue;
  }
  if( value>maxValue )
  {
    success = false;
    value = maxValue;
  }

  currentValue = value;
  upToDate = false;

  return success;
}


/*
 * Updates the locally stored value from the physical
 * thermostat over the network.
 *
 * Success or not is stored in member variable upToDate.
 *
 * Most of the method is constructing the neccessary
 * system calls.
 */
void Thermostat::updateLocalValueFromSlave()
{
  std::stringstream systemCall;
  string tmpFile;

  systemCall <<"/tmp/heating-master"<<this;
  tmpFile = systemCall.str();
  systemCall.str("");

  systemCall << "coap-client -m get coap://[";

  for( unsigned i = 0; i<7; i++ )
  {
    systemCall <<std::hex<<ipv6Address.blocks[i];
    systemCall <<":";
  }
  systemCall <<std::hex<<ipv6Address.blocks[7];

  systemCall << "]:5683/actuators/servo -B 5 2>/dev/null | sed '1d' 2>/dev/null >"<<tmpFile;

  bool lUpToDate = false;
  if( system( systemCall.str().c_str() ) == 0)
  {
    json_value* jValue = parseJsonFile(tmpFile);
    if(jValue)
    {
      try
      {
        currentValue = stoul((const char*)(*jValue)["servo1"]);
        lUpToDate = true;
      }
      catch(...)
      {

      }
    }
  }

  systemCall.str("");

  systemCall<<"rm "<<tmpFile;

  system( systemCall.str().c_str() );

  upToDate = lUpToDate;

}


/*
 * Updates the value of the physical thermostat from the
 * locally stored value over the network.
 *
 * Success or not is not stored because it cannot be
 * obtained this way. To be shure, you should call
 * updateLocalValueFromSlave() after this method.
 *
 * Most of the method is constructing the neccessary
 * system calls.
 */
void Thermostat::updateSlaveFromLocalValue()
{
  std::stringstream systemCall;

  systemCall << "coap-client -m put coap://[";

  for( unsigned i = 0; i<7; i++ )
  {
    systemCall <<std::hex<<ipv6Address.blocks[i];
    systemCall <<":";
  }
  systemCall <<std::hex<<ipv6Address.blocks[7];

  systemCall << "]:5683/actuators/servo -e \"servo1="<<std::dec<<currentValue<<"\" -B 5 2>/dev/null >/dev/null";

  system( systemCall.str().c_str() );
}

