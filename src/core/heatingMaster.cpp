/*
 * heatingMaster.cpp
 *
 *  Created on: 2015-02
 *      Author: D.Rabel
 *     License: See LICENSE.txt in the root folder of this project. //todo lizenz abklären
 */

#include <fstream>
#include <iostream>
#include <iomanip>
#include <ios>
#include "../core/heatingMaster.hpp"

// todo comment your *.cpp files
HeatingMaster::HeatingMaster(string configFile)
{
  json_value* jValue = parseJsonFile(configFile);

  // todo fehlerbehandlung

  const char* ipv6Prefix = (*jValue)["IPv6 Prefix"];

  for(int i = 0; (*jValue)["Slaves"][i].type!=json_none; i++)
  {
    const char* mac = (*jValue)["Slaves"][i]["MAC"];
    Ipv6Address ipv6 = prefixAndMacToIpAddress(ipv6Prefix, mac);

    Thermostat* newThermostat = new Thermostat(ipv6,(json_int_t)(*jValue)["Slaves"][i]["minValue"], (json_int_t)(*jValue)["Slaves"][i]["maxValue"], (const char *)(*jValue)["Slaves"][i]["name"]);

    thermostats.push_back(newThermostat);
  }
}

HeatingMaster::~HeatingMaster()
{
  for( unsigned i=0; i<thermostats.size(); i++ )
  {
    delete (thermostats[i]);
  }
}

void HeatingMaster::printHeadline( bool extended ) const
{
  cout<<"\nNum. "<<"Connected"<<setw(7)<<"Min"<<setw(7)<<"Curr"<<setw(7)<<"Max";
  if( extended )
  {
    cout<<setw(44)<<"IP address";
  }
  cout<<setw(8)<<"Name\n"<<endl;

}

void HeatingMaster::printThermostatList( bool extended ) const
{

  printHeadline(extended);
  for( unsigned i = 0; i<thermostats.size(); i++ )
    printThermostat(i, extended);

  cout<<endl;
}
void HeatingMaster::printThermostat(unsigned thermostatNumber, bool extended) const
{

  //cout<<setw(4)<<thermostatNumber+1<<": [";
  cout<<setw(4)<<thermostatNumber+1<<": ";
  // todo: fehlerbehandlung wg index
  Thermostat& tmpThermostatReference = *(thermostats[thermostatNumber]);


//  Ipv6Address ipv6 = tmpThermostatReference.getIpv6Address();
//  for( int i=0; i<7; i++)
//    cout<<setfill('0')<<setw(4)<<hex<<ipv6.blocks[i]<<":";
//  cout<<hex<<ipv6.blocks[7]<<"],  "<<setfill(' ')<<dec;
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

const unsigned HeatingMaster::getNumberOfThermostats() const
{
  return thermostats.size();
}

Thermostat * const HeatingMaster::getThermostat(unsigned thermostatNumber) const
{
  return thermostats[thermostatNumber];
}

const bool HeatingMaster::isInitialized() const
{
  bool retVal = true;
  for( unsigned i=0; i<thermostats.size(); i++)
  {
    if( thermostats[i]->isInitialized() == false )
    {
      retVal = false;
    }
  }

  return retVal;
}



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
