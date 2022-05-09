/**
 * Copyright (c) 2022 LXRobotics GmbH.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/l3xz/graphs/contributors.
 */

#ifndef ROBOT_STATE_H_
#define ROBOT_STATE_H_

/**************************************************************************************
 * INCLUDES
 **************************************************************************************/

#include "RobotStateInput.h"
#include "RobotStateOutput.h"

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class RobotState
{
public:
  enum class Name
  {
    BackwardWalking,
    ForwardWalking,
    InitState,
    StandingState,
    TurningLeft,
    TurningRight
  };

  virtual ~RobotState() { }
  virtual Name name() const = 0;
  virtual void onEnter() { }
  virtual void onExit() { }
  virtual RobotState * update(RobotStateInput const & input, RobotStateOutput & output) = 0;
};

#endif /* ROBOT_STATE_H_ */