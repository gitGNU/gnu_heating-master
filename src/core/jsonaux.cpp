/*
 * jsonaux.cpp
 *
 *  Created on: 2015-03
 *      Author: D.Rabel
 *     License: See LICENSE.txt in the root folder of this project. //todo lizenz abklären
 */

using namespace std; //todo einheitlich oder gar nich

#include "../core/jsonaux.hpp"

json_value* parseJsonFile(string fileName)

{
  string jsonString = "";

  ifstream jsonFile(fileName);

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

  return json_parse(jsonString.c_str(), jsonString.size());
}
