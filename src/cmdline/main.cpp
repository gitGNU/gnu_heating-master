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
  cout<<"\nheating-master Help \n";
  cout<<"\nCall without arguments: Print list of thermostats\n\n";
  cout<<"Arguments:\n";
  cout<<"-c 'filename': Use 'filename' instead of standard config file\n";
  cout<<"-n 'number'  : Only print (or set) thermostat with number 'number'\n";
  cout<<"-s 'value'   : Set thermostat value to 'value'. Only works together with -n\n";
  cout<<"-e           : Show extended information when printing thermostat(s)\n";
  cout<<"-h           : This help\n";
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
  bool    extended  = false;    /* Extended or not from the command line */
  bool    help      = false;    /* Print help? from the command line */
  int     opt;                  /* Variable for command line options */
  thread* extraThread(0);       /* Extra thread for parallel updating of thermostats */

  /*
   * Process command line options
   */
  while( (opt=getopt(argc, argv, "c:n:s:eh")) != EOF )
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
  else
  {
    /* No filename from the command line? Use default config file. */
    if( filename.empty() )
      filename = (string)getenv("HOME")+"/.heating-master/config.json";

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
