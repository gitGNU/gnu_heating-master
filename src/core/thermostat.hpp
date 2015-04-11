/*
 * thermostat.hpp
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

#ifndef SRC_CORE_THERMOSTAT_HPP_
#define SRC_CORE_THERMOSTAT_HPP_

#include <cstdint>
#include <string>

typedef struct {
  uint16_t blocks[8];
} Ipv6Address;


class Thermostat
{
  /*
   * Represantation of a thermostat node in the 6LoWPAN network
   */

  public:

    /* Constructor */
    Thermostat(Ipv6Address ip, unsigned min, unsigned max, std::string name) :
      ipv6Address(ip),
      minValue(min),
      maxValue(max),
      name(name),
      currentValue((min+max)/2)
    {

    }

    /* Getters and Setters */
    const Ipv6Address&  getIpv6Address()  const { return ipv6Address; }
    const unsigned      getMaxValue()     const { return maxValue; }
    const unsigned      getMinValue()     const { return minValue; }
    const std::string&  getName()         const { return name; }
    const unsigned      getCurrentValue() const { return currentValue; }
    const unsigned      isUpToDate()      const { return upToDate; }      /* upToDate means synchronized with physical thermostat */

    bool setCurrentValue( unsigned value );

    /* Methods */
    void updateLocalValueFromSlave();
    void updateSlaveFromLocalValue();


  private:
    /* Private member variables */
    const Ipv6Address   ipv6Address;
    const unsigned      minValue, maxValue;
    const std::string   name;
          unsigned      currentValue;
          bool          upToDate = false;

};


#endif /* SRC_CORE_THERMOSTAT_HPP_ */
