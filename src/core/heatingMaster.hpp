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

#include "../core/jsonaux.hpp"
#include "../core/thermostat.hpp"

class HeatingMaster
{
  public:
    /* Constructor and Destructor */
    HeatingMaster(string configFile);
    ~HeatingMaster();

    /* Print to the standard output */
    void printThermostatList (bool extended)                            const;
    void printThermostat     (unsigned thermostatNumber, bool extended) const;
    void printHeadline       (bool extended)                            const;

    /* Getters */
    const bool         isInitialized()                          const;
    const unsigned     getNumberOfThermostats()                 const;
    Thermostat * const getThermostat(unsigned thermostatNumber) const;


  private:
    /* The actual vector of thermostats */
    std::vector<Thermostat*>  thermostats;
    /* Static function to obtain ip address from prefix and mac address string */
    static Ipv6Address prefixAndMacToIpAddress(const char* prefix, const char* mac);
};



#endif /* SRC_CORE_HEATINGMASTER_HPP_ */
