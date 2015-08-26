/*
 * guiMain.cpp
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

#include <getopt.h>
#include <gtkmm.h>
#include <iostream>
#include <string>
#include <config.h>
#include "mainWindow.hpp"

using namespace std;

/*
 * Print help to the screen
 */
void printHelp()
{
  cout<<"\nUsage:\n  heating-master-gui [options]\n";
  cout<<"\nOptions:\n";
  cout<<"  -c, --config <file>     : Use <file> instead of standard config file\n";
  cout<<"  -s, --style <file>      : Use <file> instead of standard style file\n";
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
  cout<<"\nheating-master-gui "<<VERSION<<"\n";
  cout<<endl;
}


int main(int argc, char** argv)
{
  string  config    = "";       /* Filename for config file */
  string  style     = "";
  bool    help      = false;    /* Print help? from the command line */
  bool    version   = false;    /* Print version string? from the command line */
  int     opt, option_index;    /* Variables for command line options */

  /* long version of cmdline options */
  struct option long_options[] =
  {
    {"config",    required_argument, 0, 'c'},
    {"style",     required_argument, 0, 's'},
    {"help",      no_argument,       0, 'h'},
    {"version",   no_argument,       0, 'v'},
    {0, 0, 0, 0}
  };

  /*
   * Process command line options
   */
  while( (opt=getopt_long(argc, argv, "c:s:hv", long_options, &option_index)) != EOF )
  {
    switch(opt)
    {
      case 'c':
        config = optarg;
        break;

      case 's':
        style = optarg;
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
    /*
     * Build GTK+ Application
     */
    int argc1 = 1;
    Glib::RefPtr<Gtk::Application> app =
      Gtk::Application::create(argc1, argv,
        "com.noresoft.heating-master-gui");

    /* Initialize Glib threads */
    if(!Glib::thread_supported()) Glib::thread_init();

    /* No filename from the command line? Use default config file. */
    if( config.empty() )
      config = CONF_PATH "/heating-master.conf";

    /* Initialize heating master */
    HeatingMaster heatingMaster(config);

    /* No filename from the command line? Use default style file. */
    if( style.empty() )
      style = CONF_PATH "/style.css";

    /* Initialize main window */
    MainWindow window(heatingMaster, style);

    /* run */
    return app->run(window);
  }

  return 0;
}



