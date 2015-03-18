/*
 * main.cpp
 *
 *  Created on: 2015-02
 *      Author: D.Rabel
 *     License: See LICENSE.txt in the root folder of this project.
 */

#include <getopt.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <chrono>

#include "../core/thermostat.hpp"
#include "../core/heatingMaster.hpp"


int main( int argc, char *argv[] )
{
  string filename = "";
  int setValue = -1;
  int number = -1;
  bool extended = false;
  int opt;

  while( (opt=getopt(argc, argv, "c:n:s:e")) != EOF )
  {
    switch(opt)
    {
      case 'c':
        filename = optarg;
        break;

      case 'n':
        if(optarg)
          number = atoi(optarg);
        break;

      case 's':
        if(optarg)
          setValue = atoi(optarg);
        break;

      case 'e':
        extended = true;
        break;

      case '?':
      default:
        break;
    }
  }

  if( filename.empty() )
    filename = (string)getenv("HOME")+"/.heating-master/config.json";

  HeatingMaster heatingMaster(filename);

  //std::this_thread::sleep_for(std::chrono::seconds(3));

  while(!heatingMaster.isInitialized()){ cout<<"."<<flush; std::this_thread::sleep_for(std::chrono::seconds(1)); }; // don't wait this long. todo don't use isInitialized()
  cout<<endl;

  //todo try catch und exceptions
  if( number == -1 )
  {
    heatingMaster.printThermostatList(extended);
    return 0;
  }
  if( setValue == -1 )
  {
    heatingMaster.printHeadline(extended);
    heatingMaster.printThermostat((unsigned)number-1, extended);
    cout<<endl;
    return 0;
  }
  heatingMaster.getThermostat((unsigned)number-1)->setCurrentValue((unsigned)setValue, nullptr);
  while(heatingMaster.getThermostat((unsigned)number-1)->isUpdating()) { cout<<"."<<flush; std::this_thread::sleep_for(std::chrono::seconds(1)); };
  cout<<endl;
  if(heatingMaster.getThermostat((unsigned)number-1)->isUpToDate())
  {
    cout<<"hat jeklappt"<<endl;
  }
  else
  {
    cout<<"hat nich geklappt"<<endl;
  }


  return 0;
}
