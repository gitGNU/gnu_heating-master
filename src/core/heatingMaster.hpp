/*
 * heatingMaster.hpp
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
