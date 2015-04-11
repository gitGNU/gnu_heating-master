/*
 * heatingMaster.hpp
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

#ifndef SRC_CORE_HEATINGMASTER_HPP_
#define SRC_CORE_HEATINGMASTER_HPP_

#include <vector>
#include <string>
#include "jsonaux.hpp"
#include "thermostat.hpp"

class HeatingMaster
{
  /*
   * Control class, that initializes and holds a list of the thermostats
   * specified in the config file
   */

  public:
    /* Constructor and Destructor */
    HeatingMaster(std::string configFile);
    ~HeatingMaster();

    /* Print to the standard output */
    void printThermostatList (bool extended)                            const;
    void printThermostat     (unsigned thermostatNumber, bool extended) const;
    void printHeadline       (bool extended)                            const;

    /* Getters */
    const unsigned     getNumberOfThermostats()                 const;
    Thermostat * const getThermostat(unsigned thermostatNumber) const;


  private:
    /* The actual vector of thermostats */
    std::vector<Thermostat*>  thermostats;
    /* Static method to obtain ip address from prefix and mac address string */
    static Ipv6Address prefixAndMacToIpAddress(const char* prefix, const char* mac);
};



#endif /* SRC_CORE_HEATINGMASTER_HPP_ */
