/*
 * thermostat.hpp
 *
 *  Created on: 2015-02
 *      Author: D.Rabel
 *     License: See LICENSE.txt in the root folder of this project.
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
