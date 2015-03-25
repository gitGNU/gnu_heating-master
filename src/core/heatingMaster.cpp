/*
 * heatingMaster.cpp
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

#include <fstream>
#include <iostream>
#include <iomanip>
#include <ios>
#include "heatingMaster.hpp"

using namespace std;

/*
 * Constructor
 *
 * Initializes thermostats from config file (in JSON)
 */
HeatingMaster::HeatingMaster(string configFile)
{
  json_value* jValue = parseJsonFile(configFile);

  if( jValue )
  {
    const char* ipv6Prefix = (*jValue)["IPv6 Prefix"];

    for(int i = 0; (*jValue)["Slaves"][i].type!=json_none; i++)
    {
      const char* mac = (*jValue)["Slaves"][i]["MAC"];
      Ipv6Address ipv6 = prefixAndMacToIpAddress(ipv6Prefix, mac);

      Thermostat* newThermostat = new Thermostat(ipv6,(json_int_t)(*jValue)["Slaves"][i]["minValue"], (json_int_t)(*jValue)["Slaves"][i]["maxValue"], (const char *)(*jValue)["Slaves"][i]["name"]);

      thermostats.push_back(newThermostat);
    }
  }
}

/*
 * Destructor
 *
 * Deletes thermostat objects from heap
 */
HeatingMaster::~HeatingMaster()
{
  for( unsigned i=0; i<thermostats.size(); i++ )
  {
    delete (thermostats[i]);
  }
}

/*
 * Prints a head line on the command line
 */
void HeatingMaster::printHeadline( bool extended ) const
{
  cout<<"\nNum. "<<"Connected"<<setw(7)<<"Min"<<setw(7)<<"Curr"<<setw(7)<<"Max";
  if( extended )
  {
    cout<<setw(44)<<"IP address";
  }
  cout<<setw(8)<<"Name\n"<<endl;
}

/*
 * Prints a list of the thermostats on the command line
 */
void HeatingMaster::printThermostatList( bool extended ) const
{

  printHeadline(extended);
  for( unsigned i = 0; i<thermostats.size(); i++ )
    printThermostat(i, extended);

  cout<<endl;
}

/*
 * Prints one thermostat on the command line
 */
void HeatingMaster::printThermostat(unsigned thermostatNumber, bool extended) const
{

  cout<<setw(4)<<thermostatNumber+1<<": ";

  Thermostat& tmpThermostatReference = *(thermostats[thermostatNumber]);

  cout<<setw(8)<<(tmpThermostatReference.isUpToDate()?"yes":"no")<<",  ";
  cout<<setw(4)<<tmpThermostatReference.getMinValue()    <<",  ";
  cout<<setw(4)<<tmpThermostatReference.getCurrentValue()<<",  ";
  cout<<setw(4)<<tmpThermostatReference.getMaxValue()    <<",  ";
  if( extended )
  {
    cout<<"[";
    Ipv6Address ipv6 = tmpThermostatReference.getIpv6Address();
    for( unsigned i=0; i<7; i++)
      cout<<setfill('0')<<setw(4)<<hex<<ipv6.blocks[i]<<":";
    cout<<hex<<ipv6.blocks[7]<<"],  "<<setfill(' ')<<dec;
  }
  cout<<"("<<tmpThermostatReference.getName()<<")"<<endl;

}

/*
 * Returns size of thermostat vector, i.e. number of thermostats
 */
const unsigned HeatingMaster::getNumberOfThermostats() const
{
  return thermostats.size();
}

/*
 * Returns a pointer to a certain thermostat (be careful with this)
 */
Thermostat * const HeatingMaster::getThermostat(unsigned thermostatNumber) const
{
  return thermostats[thermostatNumber];
}

/*
 * Auxiliary static method to obtain ip address from prefix and mac address string
 */
Ipv6Address HeatingMaster::prefixAndMacToIpAddress(const char* prefix, const char* mac)
{
  Ipv6Address ipv6;

  char byte[5];

  byte[4]=0;

  for( int i=0; i<3; i++ )
  {
    memcpy(byte, prefix+i*5, 4);
    ipv6.blocks[i] = (uint16_t)strtol(byte, 0, 16);
  }
  ipv6.blocks[3] = 0;

  byte[2]=0;

  for( int i=0; i<4; i++ )
  {
    memcpy(byte, mac+i*6+3, 2);
    ipv6.blocks[i+4] = (uint16_t)strtol(byte, 0, 16);
    memcpy(byte, mac+i*6, 2);
    ipv6.blocks[i+4] += (uint16_t)strtol(byte, 0, 16)*0x100;
  }

  ipv6.blocks[4] ^= 0x200;

  return ipv6;
}
