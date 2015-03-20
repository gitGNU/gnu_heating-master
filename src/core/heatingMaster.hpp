/*
 * heatingMaster.hpp
 *
 *  Created on: 2015-02
 *      Author: D.Rabel
 *     License: See LICENSE.txt in the root folder of this project.
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
   * Control class, that initializes and hold a list of the thermostats
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
