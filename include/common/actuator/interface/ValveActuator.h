/**
 * Copyright (c) 2022 LXRobotics GmbH.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/l3xz/graphs/contributors.
 */

#ifndef COMMON_ACTUATOR_INTERFACE_VALVE_ACTUATOR_H_
#define COMMON_ACTUATOR_INTERFACE_VALVE_ACTUATOR_H_

/**************************************************************************************
 * INCLUDES
 **************************************************************************************/

#include "Base.h"

#include <memory>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace common::actuator::interface
{

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class ValveActuator : public Base
{
public:
           ValveActuator(std::string const & name) : Base{name} { }
  virtual ~ValveActuator() { }

  virtual void set(float const val) = 0;
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

typedef std::shared_ptr<ValveActuator> SharedValveActuator;

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* common::actuator::interface */

#endif /* COMMON_ACTUATOR_INTERFACE_VALVE_ACTUATOR_H_ */