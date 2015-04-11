/*
 * jsonaux.cpp
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
 *    along with Heating Master. If not, see <http://www.gnu.org/licenses/>.
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
