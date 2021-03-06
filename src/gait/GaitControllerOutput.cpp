/**
 * Copyright (c) 2022 LXRobotics GmbH.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/l3xz_io/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDES
 **************************************************************************************/

#include <gait/GaitControllerOutput.h>

#include <sstream>
#include <stdexcept>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace gait
{

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

ControllerOutput::ControllerOutput(float const left_front_coxa_angle_target,
                                   float const left_front_femur_angle_target,
                                   float const left_front_tibia_angle_target,
                                   float const left_middle_coxa_angle_target,
                                   float const left_middle_femur_angle_target,
                                   float const left_middle_tibia_angle_target,
                                   float const left_back_coxa_angle_target,
                                   float const left_back_femur_angle_target,
                                   float const left_back_tibia_angle_target,
                                   float const right_front_coxa_angle_target,
                                   float const right_front_femur_angle_target,
                                   float const right_front_tibia_angle_target,
                                   float const right_middle_coxa_angle_target,
                                   float const right_middle_femur_angle_target,
                                   float const right_middle_tibia_angle_target,
                                   float const right_back_coxa_angle_target,
                                   float const right_back_femur_angle_target,
                                   float const right_back_tibia_angle_target)
{
  _angle_position_map[make_key(Leg::LeftFront,   Joint::Coxa)]  = left_front_coxa_angle_target;
  _angle_position_map[make_key(Leg::LeftFront,   Joint::Femur)] = left_front_femur_angle_target;
  _angle_position_map[make_key(Leg::LeftFront,   Joint::Tibia)] = left_front_tibia_angle_target;

  _angle_position_map[make_key(Leg::LeftMiddle,  Joint::Coxa)]  = left_middle_coxa_angle_target;
  _angle_position_map[make_key(Leg::LeftMiddle,  Joint::Femur)] = left_middle_femur_angle_target;
  _angle_position_map[make_key(Leg::LeftMiddle,  Joint::Tibia)] = left_middle_tibia_angle_target;

  _angle_position_map[make_key(Leg::LeftBack,    Joint::Coxa)]  = left_back_coxa_angle_target;
  _angle_position_map[make_key(Leg::LeftBack,    Joint::Femur)] = left_back_femur_angle_target;
  _angle_position_map[make_key(Leg::LeftBack,    Joint::Tibia)] = left_back_tibia_angle_target;

  _angle_position_map[make_key(Leg::RightFront,  Joint::Coxa)]  = right_front_coxa_angle_target;
  _angle_position_map[make_key(Leg::RightFront,  Joint::Femur)] = right_front_femur_angle_target;
  _angle_position_map[make_key(Leg::RightFront,  Joint::Tibia)] = right_front_tibia_angle_target;

  _angle_position_map[make_key(Leg::RightMiddle, Joint::Coxa)]  = right_middle_coxa_angle_target;
  _angle_position_map[make_key(Leg::RightMiddle, Joint::Femur)] = right_middle_femur_angle_target;
  _angle_position_map[make_key(Leg::RightMiddle, Joint::Tibia)] = right_middle_tibia_angle_target;

  _angle_position_map[make_key(Leg::RightBack,   Joint::Coxa)]  = right_back_coxa_angle_target;
  _angle_position_map[make_key(Leg::RightBack,   Joint::Femur)] = right_back_femur_angle_target;
  _angle_position_map[make_key(Leg::RightBack,   Joint::Tibia)] = right_back_tibia_angle_target;
}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

float ControllerOutput::get_angle_deg(Leg const leg, Joint const joint) const
{
  if (!_angle_position_map.count(make_key(leg, joint)))
  {
    std::stringstream msg;

    msg << "ControllerOutput::operator(): error, trying to access non-existing position actuator ("
        << static_cast<int>(leg)
        << ", "
        << static_cast<int>(joint)
        << ")";

    throw std::runtime_error(msg.str());
  }

  return _angle_position_map.at(make_key(leg, joint));
}

void ControllerOutput::set_angle_deg(Leg const leg, Joint const joint, float const angle_deg)
{
  _angle_position_map[make_key(leg, joint)] = angle_deg;
}

std::string ControllerOutput::toStr() const
{
  std::stringstream msg;

  msg << "\n"
      << "Left\n"
      << "  Front :"
      << "  Coxa: "   << _angle_position_map.at(make_key(Leg::LeftFront, Joint::Coxa))
      << "  Femur: "  << _angle_position_map.at(make_key(Leg::LeftFront, Joint::Femur))
      << "  Tibia: "  << _angle_position_map.at(make_key(Leg::LeftFront, Joint::Tibia))
      << "\n"
      << "  Middle:"
      << "  Coxa: "   << _angle_position_map.at(make_key(Leg::LeftMiddle, Joint::Coxa))
      << "  Femur: "  << _angle_position_map.at(make_key(Leg::LeftMiddle, Joint::Femur))
      << "  Tibia: "  << _angle_position_map.at(make_key(Leg::LeftMiddle, Joint::Tibia))
      << "\n"
      << "  Back  :"
      << "  Coxa: "   << _angle_position_map.at(make_key(Leg::LeftBack, Joint::Coxa))
      << "  Femur: "  << _angle_position_map.at(make_key(Leg::LeftBack, Joint::Femur))
      << "  Tibia: "  << _angle_position_map.at(make_key(Leg::LeftBack, Joint::Tibia))
      << "\n"
      << "Right\n"
      << "  Front :"
      << "  Coxa: "   << _angle_position_map.at(make_key(Leg::RightFront, Joint::Coxa))
      << "  Femur: "  << _angle_position_map.at(make_key(Leg::RightFront, Joint::Femur))
      << "  Tibia: "  << _angle_position_map.at(make_key(Leg::RightFront, Joint::Tibia))
      << "\n"
      << "  Middle:"
      << "  Coxa: "   << _angle_position_map.at(make_key(Leg::RightMiddle, Joint::Coxa))
      << "  Femur: "  << _angle_position_map.at(make_key(Leg::RightMiddle, Joint::Femur))
      << "  Tibia: "  << _angle_position_map.at(make_key(Leg::RightMiddle, Joint::Tibia))
      << "\n"
      << "  Back  :"
      << "  Coxa: "   << _angle_position_map.at(make_key(Leg::RightBack, Joint::Coxa))
      << "  Femur: "  << _angle_position_map.at(make_key(Leg::RightBack, Joint::Femur))
      << "  Tibia: "  << _angle_position_map.at(make_key(Leg::RightBack, Joint::Tibia));

  return msg.str();
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* gait */
