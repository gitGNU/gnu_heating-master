/*
 * jsonaux.cpp
 *
 *  Created on: 2015-03
 *      Author: D.Rabel
 *     License: See LICENSE.txt in the root folder of this project.
 */

#include <iostream>
#include "jsonaux.hpp"

using namespace std;

/* Wrapper to pass a filename instead of a JSON string to the JSON parser */
json_value* parseJsonFile(string fileName)
{
  string jsonString = "";

  ifstream jsonFile(fileName);

  /* Build string from file input. If file does not exist, the string will be empty */
  if(jsonFile.is_open())
  {
    string line;
    while( getline(jsonFile, line) )
    {
      jsonString.append(line);
      jsonString.append("\n");
    }
    jsonFile.close();
  }

  /* Call JSON parser with the built string and return the JSON structure */
  return json_parse(jsonString.c_str(), jsonString.size());
}
