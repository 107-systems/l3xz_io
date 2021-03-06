/**
 * Copyright (c) 2022 LXRobotics GmbH.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/l3xz_io/graphs/contributors.
 */

#ifndef COMMON_ACTUATOR_INTERFACE_BASE_HPP_
#define COMMON_ACTUATOR_INTERFACE_BASE_HPP_

/**************************************************************************************
 * INCLUDES
 **************************************************************************************/

#include <string>
#include <optional>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace common::actuator::interface
{

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

template <typename T>
class Base
{
public:
           Base(std::string const & name) : _name{name} { }
  virtual ~Base() { }

  virtual void set(T const & val) = 0;
  virtual std::string toStr() const;
  inline std::string name() const { return _name; }

protected:
  virtual std::optional<T> get() const = 0;

private:
  std::string const _name;
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* common::actuator::interface */

/**************************************************************************************
 * TEMPLATE IMPLEMENTATION
 **************************************************************************************/

#include "Base.ipp"

#endif /* COMMON_ACTUATOR_INTERFACE_BASE_HPP_ */
