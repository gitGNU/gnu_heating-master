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
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include "thermostatObserver.hpp"

using namespace std;

typedef struct {
  uint16_t blocks[8];
} Ipv6Address;

struct  ThermostatCallback {
  void* objectPointer;
  void (* callbackFunction)(void*);
};

class Thermostat
{
  /*
   * Represantation of a thermostat node in the 6LoWPAN network
   */

  public:

    /* Constructor */
    Thermostat(Ipv6Address ip, unsigned min, unsigned max, string name) :
      ipv6Address(ip),
      minValue(min),
      maxValue(max),
      name(name),
      currentValue((min+max)/2)
    {
      updateLocalValueFromSlave({0, 0});
    }

    /* Delete Copy- and Move-Constructor because of own thread */
    Thermostat(const Thermostat&) = delete;
    Thermostat(Thermostat&&)      = delete;

    /* Destructor */
    ~Thermostat()
    {
      if( updateThread.joinable() )
      {
        updateThread.join();
      }
    }

    void attachObserver(ThermostatObserver* observer);


    /* Getters and Setters */
    const Ipv6Address&  getIpv6Address()  const { return ipv6Address; }
    const unsigned      getMaxValue()     const { return maxValue; }
    const unsigned      getMinValue()     const { return minValue; }
    const string&       getName()         const { return name; }
    const unsigned      getCurrentValue() const { return currentValue; }
    const bool          isUpToDate()      const { return upToDate; }
    const bool          isUpdating()      const { return updating; }
    const bool          isInitialized()   const { return initialized; }

    void setCurrentValue( unsigned value, ThermostatCallback callback );

    /* Methods */
    void updateLocalValueFromSlave( ThermostatCallback callback );
    void updateSlaveFromLocalValue( ThermostatCallback callback );


  private:
    const Ipv6Address   ipv6Address;
    const unsigned      minValue, maxValue;
    const string        name;
          unsigned      currentValue;
          bool          upToDate    = false;
          bool          updating    = false;
          bool          initialized = false; // todo verschwinden lassen. depricated ;-)
          thread        updateThread;

    vector<ThermostatObserver*> observers;

    /* Methods for updating value in extra thread */
    void updateLocalValueFromSlaveThread( ThermostatCallback callback );
    void updateSlaveFromLocalValueThread( ThermostatCallback callback );
};


#endif /* SRC_CORE_THERMOSTAT_HPP_ */
