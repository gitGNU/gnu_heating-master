/*
 * thermostatObserver.hpp
 *
 *  Created on: 16.03.2015
 *      Author: nore (D.Rabel)
 *     License: GPLv2 or later todo
 */

#ifndef SRC_CORE_THERMOSTATOBSERVER_HPP_
#define SRC_CORE_THERMOSTATOBSERVER_HPP_


class ThermostatObserver
{
  public:
    virtual void thermostatStateUpdate(bool, bool, unsigned) = 0;

    virtual ~ThermostatObserver()
    {

    }
};


#endif /* SRC_CORE_THERMOSTATOBSERVER_HPP_ */
