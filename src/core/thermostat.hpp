/*
 * thermostat.hpp
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
