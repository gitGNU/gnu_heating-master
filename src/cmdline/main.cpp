/*
 * main.cpp
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

#include <getopt.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "../core/thermostat.hpp"
#include "../core/heatingMaster.hpp"

using namespace std;

/*
 * Global variable and functions to handle in parallel thread
 */
static bool updating = false;

/* Update all local thermostat values over the network */
void updateAll(HeatingMaster* heatingMaster)
{
  for( unsigned i = 0; i<heatingMaster->getNumberOfThermostats(); i++ )
  {
    heatingMaster->getThermostat(i)->updateLocalValueFromSlave();
  }
  updating = false;
}
/* Update one local thermostat value over the network */
void updateOne(Thermostat* thermostat)
{
  thermostat->updateLocalValueFromSlave();
  updating = false;
}
/* Set one local thermostat value over the network */
void setOne(Thermostat* thermostat)
{
  thermostat->updateSlaveFromLocalValue();
  updating = false;
}

/*
 * Print help to the screen
 */
void printHelp()
{
  cout<<"\nUsage:\n  heating-master [options]\n";
  cout<<"\nCall without options:\n  Print list of thermostats\n";
  cout<<"\nOptions:\n";
  cout<<"  -c, --config <file>     : Use <file> instead of standard config file\n";
  cout<<"  -n, --number <number>   : Only print (or set) thermostat with number <number>\n";
  cout<<"  -s, --set-value <value> : Set thermostat value to 'value'.\n"
        "                            Only works together with -n (--numbers)\n";
  cout<<"  -e, --extended          : Show extended thermostat information\n";
  cout<<"  -v, --version           : Print version string\n";
  cout<<"  -h, --help              : Print this help\n";
  cout<<endl;
}

/*
 * Print version string to the screen
 */
void printVersion()
{
#ifndef VERSION
#define VERSION "version unknown"
#endif
  cout<<"\nheating-master "<<VERSION<<"\n";
  cout<<endl;
}

/*
 * Main
 */
int main( int argc, char *argv[] )
{
  string  filename  = "";       /* Filename for config file */
  int     setValue  = -1;       /* Value to set from the command line */
  int     number    = -1;       /* Thermostat number from the command line */
  bool    extended  = false;    /* Extended or not? from the command line */
  bool    help      = false;    /* Print help? from the command line */
  bool    version   = false;    /* Print version string? from the command line */ 
  int     opt, option_index;    /* Variables for command line options */
  thread* extraThread(0);       /* Extra thread for parallel updating of thermostats */

  /* long version of cmdline options */
  struct option long_options[] =
  {
    {"config",    required_argument, 0, 'c'},
    {"number",    required_argument, 0, 'n'},
    {"set-value", required_argument, 0, 's'},
    {"extended",  no_argument,       0, 'e'},
    {"help",      no_argument,       0, 'h'},
    {"version",   no_argument,       0, 'v'},
    {0, 0, 0, 0}
  };

  /*
   * Process command line options
   */
  while( (opt=getopt_long(argc, argv, "c:n:s:ehv", long_options, &option_index)) != EOF )
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

      case 'h':
        help = true;
        break;

      case 'v':
        version = true;
        break;

      case '?':
      default:
        break;
    }
  }

  /* Print help to the screen  */
  if( help )
  {
    printHelp();
  }
  /* Print version string to the screen  */
  else if( version )
  {
    printVersion();
  }
  else
  {
    /* No filename from the command line? Use default config file. */
    if( filename.empty() )
      filename = "/etc/heating-master/heating-master.conf";

    /* Initialize HeatingMaster object from config file. */
    HeatingMaster heatingMaster(filename);

    /* No number argument passed? Print all thermostats. */
    if( number == -1 )
    {
      updating = true;                                                  /* set updating flag */
      extraThread = new thread( &updateAll, &heatingMaster );           /* start new thread to update thermostats */
      while(updating)                                                   /* wait until thread has finished */
      {
        cout<<"."<<flush; this_thread::sleep_for(chrono::seconds(1));   /* print a . on the commandline every second */
      };
      extraThread->join();                                              /* join the thread to be shure it is done */
      heatingMaster.printThermostatList(extended);                      /* print thermostat list */
    }
    else
    /* No value passed? Print the thermostat. */
    if( setValue == -1 )
    {
      unsigned n = number-1;                                            /* to make the following code more clear */
      /* Print a message if the thermostat number is too high */
      if( n > heatingMaster.getNumberOfThermostats() )
      {
        cout<<"Only ";
        cout<<heatingMaster.getNumberOfThermostats();
        cout<<" thermostats available"<<endl;
        return -1;
      }

      updating = true;                                                  /* set updating flag */
      extraThread =                                                     /* start new thread to update thermostat */
          new thread( &updateOne, heatingMaster.getThermostat(n) );
      while(updating)                                                   /* wait until thread has finished */
      {
        cout<<"."<<flush; this_thread::sleep_for(chrono::seconds(1));   /* print a . on the commandline every second */
      };
      extraThread->join();                                              /* join the thread to be shure it is done */

      heatingMaster.printHeadline(extended);                            /* print head line */
      heatingMaster.printThermostat(n, extended);                       /* print thermostat */

    }
    else
    /* Value and number passed? Set the thermostat value. */
    {
      unsigned n = number-1;                                            /* to make the following code more clear */
      /* Print a message if the thermostat number is too high */
      if( n > heatingMaster.getNumberOfThermostats() )
      {
        cout<<"Only ";
        cout<<heatingMaster.getNumberOfThermostats();
        cout<<" thermostats available"<<endl;
        return -1;
      }

      /* try to set value in thermostat object */
      if( ! (heatingMaster.getThermostat(n)->setCurrentValue((unsigned)setValue)) )
      {
        cout<<"\nValue out of bounds, adjusting value to min or max."<<endl;
      }

      updating = true;                                                  /* set updating flag */
      extraThread =                                                     /* start new thread to transmit thermostat value */
          new thread( &setOne, heatingMaster.getThermostat(n) );
      while(updating)                                                   /* wait until thread has finished */
      {
        cout<<"."<<flush; this_thread::sleep_for(chrono::seconds(1));   /* print a . on the commandline every second */
      };
      extraThread->join();                                              /* join the thread to be shure it is done */

      /*
       * Read the value back after transmitting to see if the
       * transmission was succesful.
       * (Unfortunately the only way to ensure this at the
       *  moment.)
       */
      updating = true;                                                  /* set updating flag */
      extraThread =                                                     /* start new thread to update thermostat */
          new thread( &updateOne, heatingMaster.getThermostat(n) );
      while(updating)                                                   /* wait until thread has finished */
      {
        cout<<"."<<flush; this_thread::sleep_for(chrono::seconds(1));   /* print a . on the commandline every second */
      };
      extraThread->join();                                              /* join the thread to be shure it is done */

      /* Print to the command line, if the transmission was successful */
      cout<<endl;
      if(heatingMaster.getThermostat(n)->isUpToDate())
      {
        cout<<"Value set successfully"<<endl;
      }
      else
      {
        cout<<"Value could not be set"<<endl;
      }
    }

    cout<<endl;
  }

  return 0;
}
