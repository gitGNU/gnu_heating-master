/*
 * thermostat.cpp
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

