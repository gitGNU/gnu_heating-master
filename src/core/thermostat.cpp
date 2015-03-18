/*
 * thermostat.cpp
 *
 *  Created on: 2015-02
 *      Author: D.Rabel
 *     License: See LICENSE.txt in the root folder of this project.
 */

#include "../core/thermostat.hpp"
#include "../core/jsonaux.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <string>


void Thermostat::setCurrentValue( unsigned value, ThermostatCallback callback )
{
  if( value<minValue || value>maxValue )
  {
    //todo throw up
  }

  currentValue = value;
  updateSlaveFromLocalValue(callback);

  return;
}

void Thermostat::updateLocalValueFromSlaveThread( ThermostatCallback callback )
{
  std::stringstream systemCall;
  string tmpFile;

  systemCall <<"/tmp/heating-master"<<this;
  tmpFile = systemCall.str();
  systemCall.str("");

  systemCall << "coap-client -m get coap://["; //todo: coap-client: abrufen, verändern, bauen, dokuemntieren  // todo libcoap dazulinken statt so hier...

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
    json_value* jValue = parseJsonFile(tmpFile); //todo auch hier nicht vergessen, fehler abzufangen
    if(jValue)
    {
      currentValue = stoul((const char*)(*jValue)["servo1"]); //todo bzw hier
      lUpToDate = true;
    }
  }
//todo -g aus makefile raus
  systemCall.str("");

  systemCall<<"rm "<<tmpFile;

  system( systemCall.str().c_str() );

  // todo aus der datei lesen und casten (JSON) if (system(systemCall));


  upToDate = lUpToDate;
  initialized = true;
  updating = false;
  cout<<"ende vom thread "<<&updateThread<<endl;
  for( unsigned i = 0; i<observers.size(); i++)
  {
    observers[i]->thermostatStateUpdate(updating, upToDate, currentValue);
  }
  //if ((callback.callbackFunction) != nullptr) callback.callbackFunction(callback.objectPointer);
  // todo wert vom slave holen
  // upToDate setzen
  // ende
}

void Thermostat::updateLocalValueFromSlave( ThermostatCallback callback )
{
  if( updateThread.joinable() )
  {
    updateThread.join();
  }
  updating = true; //nach dem joinen wegen race condition, vor dem neuen thread weil nicht nebenläufig
  for( unsigned i = 0; i<observers.size(); i++)
  {
    observers[i]->thermostatStateUpdate(updating, upToDate, currentValue);
  }
  updateThread = thread(&Thermostat::updateLocalValueFromSlaveThread, this, callback);
}

void Thermostat::updateSlaveFromLocalValueThread( ThermostatCallback callback )
{
  std::stringstream systemCall;

  systemCall << "coap-client -m put coap://["; //todo: coap-client: abrufen, verändern, bauen, dokuemntieren

  for( unsigned i = 0; i<7; i++ )
  {
    systemCall <<std::hex<<ipv6Address.blocks[i];
    systemCall <<":";
  }
  systemCall <<std::hex<<ipv6Address.blocks[7];

  systemCall << "]:5683/actuators/servo -e \"servo1="<<std::dec<<currentValue<<"\" -B 5 2>/dev/null >/dev/null";

  //cout<<systemCall.str()<<endl;


  system( systemCall.str().c_str() );


  updateLocalValueFromSlaveThread(callback); //wooooouhh! das musste aber mal kommentieren oder umbennenen todo
  // todo versuchen, den aktuellen wert zu setzen.
  // falls out of range, fehler ausgeben oder so.
  // den lokal gespeicherten wert anpassen?
  // upToDate setzen
  // ende
  //todo -B seconds
}

void Thermostat::updateSlaveFromLocalValue( ThermostatCallback callback )
{
  cout<<"before joining "<<&updateThread<<endl;
  if( updateThread.joinable() )
  {
    updateThread.join();
  }
  updating = true;
  for( unsigned i = 0; i<observers.size(); i++)
  {
    observers[i]->thermostatStateUpdate(updating, upToDate, currentValue);
  }
  cout<<"after joining "<<&updateThread<<endl;
  updateThread = thread(&Thermostat::updateSlaveFromLocalValueThread, this, callback);
}

void Thermostat::attachObserver(ThermostatObserver* observer)
{
  observers.push_back(observer);
}
