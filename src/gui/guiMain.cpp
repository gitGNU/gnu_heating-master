/*
 * guiMain.cpp
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
#include <gtkmm.h>
#include <iostream>
#include <string>
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
  cout<<"  -v, --version           : Print version string\n";
  cout<<"  -h, --help              : Print this help\n";
  cout<<endl;
}

/*
 * Print version string to the screen
 */
void printVersion()
{
  cout<<"\nheating-master-gui "<<VERSION<<"\n";
  cout<<endl;
}


int main(int argc, char** argv)
{
  string  filename  = "";       /* Filename for config file */
  bool    help      = false;    /* Print help? from the command line */
  bool    version   = false;    /* Print version string? from the command line */
  int     opt, option_index;    /* Variables for command line options */

  /* long version of cmdline options */
  struct option long_options[] =
  {
    {"config",    required_argument, 0, 'c'},
    {"help",      no_argument,       0, 'h'},
    {"version",   no_argument,       0, 'v'},
    {0, 0, 0, 0}
  };

  /*
   * Process command line options
   */
  while( (opt=getopt_long(argc, argv, "c:hv", long_options, &option_index)) != EOF )
  {
    switch(opt)
    {
      case 'c':
        filename = optarg;
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
    if( filename.empty() )
      filename = "/etc/heating-master/heating-master.conf";

    /* Initialize heating master */
    HeatingMaster heatingMaster(filename);

    /* Initialize main window */
    MainWindow window(heatingMaster);

    /* run */
    return app->run(window);
  }

  return 0;
}



