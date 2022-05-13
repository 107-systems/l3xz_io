/**
 * Copyright (c) 2022 LXRobotics GmbH.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/l3xz/graphs/contributors.
 */

#ifndef TURNING_RIGHT_H_
#define TURNING_RIGHT_H_

/**************************************************************************************
 * INCLUDES
 **************************************************************************************/

#include "StateBase.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace gait::state
{

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class TurningRight : public StateBase
{
public:
  virtual ~TurningRight() { }
  virtual void onEnter() override;
  virtual void onExit() override;
  virtual StateBase * update(GaitControllerInput const & input, GaitControllerOutput & output) override;
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* gait::state */

#endif /* TURNING_RIGHT_H_ */