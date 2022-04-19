/**
 * Copyright (c) 2022 LXRobotics GmbH.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-UAVCAN/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <string>
#include <thread>
#include <chrono>
#include <sstream>

#include <ros/ros.h>
#include <ros/console.h>

#include <geometry_msgs/Twist.h>

#include <dynamixel_sdk.h>

#include <l3xz/driver/dynamixel/MX28.h>
#include <l3xz/driver/dynamixel/Dynamixel.h>

/**************************************************************************************
 * FUNCTION DECLARATION
 **************************************************************************************/

bool init_dynamixel  (l3xz::driver::SharedMX28 & mx28_ctrl);
void deinit_dynamixel(l3xz::driver::SharedMX28 & mx28_ctrl);

void cmd_vel_callback(const geometry_msgs::Twist::ConstPtr & msg);

/**************************************************************************************
 * CONSTANT
 **************************************************************************************/

static std::string const DYNAMIXEL_DEVICE_NAME = "/dev/ttyUSB0";
static float       const DYNAMIXEL_PROTOCOL_VERSION = 2.0f;
static int         const DYNAMIXEL_BAUD_RATE = 115200;

static l3xz::driver::Dynamixel::IdVect const DYNAMIXEL_ID_VECT{1,2,3,4,5,6,7,8};

static l3xz::driver::MX28::AngleDataSet const L3XZ_INITIAL_ANGLE_DATA_SET =
{
  {1, 180.0f},
  {2, 180.0f},
  {3, 180.0f},
  {4, 180.0f},
  {5, 180.0f},
  {6, 180.0f},
  {7, 180.0f},
  {8, 180.0f},
};

/**************************************************************************************
 * GLOBAL FUCKING VARIABLES (NEED TO BE FUCKING ENCAPSULATE - no time now)
 **************************************************************************************/

static l3xz::driver::MX28::AngleDataSet l3xz_mx28_target_angle = L3XZ_INITIAL_ANGLE_DATA_SET;

/**************************************************************************************
 * MAIN
 **************************************************************************************/

int main(int argc, char **argv) try
{
  ros::init(argc, argv, "l3xz");

  ros::NodeHandle node_hdl;


  std::shared_ptr<l3xz::driver::Dynamixel> dynamixel_ctrl = std::make_shared<l3xz::driver::Dynamixel>(DYNAMIXEL_DEVICE_NAME, DYNAMIXEL_PROTOCOL_VERSION, DYNAMIXEL_BAUD_RATE);
  l3xz::driver::SharedMX28 mx28_ctrl = std::make_shared<l3xz::driver::MX28>(dynamixel_ctrl);

  if (!init_dynamixel(mx28_ctrl))
    ROS_ERROR("init_dynamixel failed.");
  ROS_INFO("init_dynamixel successfully completed.");


  ros::Subscriber cmd_vel_sub = node_hdl.subscribe("/l3xz/cmd_vel", 10, cmd_vel_callback);

  for (ros::Rate loop_rate(50);
       ros::ok();
       loop_rate.sleep())
  {
    if (!mx28_ctrl->setAngle(l3xz_mx28_target_angle))
      ROS_ERROR("failed to set target angles for all dynamixel servos");

    ros::spinOnce();
/*
    mx28_ctrl->turnLedOn(opt_id_vect.value());
    loop_rate.sleep();
    mx28_ctrl->turnLedOff(opt_id_vect.value());
    loop_rate.sleep();

    l3xz::driver::MX28::AngleDataVect angle_vect_act = mx28_ctrl->getAngle(opt_id_vect.value());
    for (auto [id, angle_deg] : angle_vect_act)
      ROS_INFO("[ID:%03d] Angle Act = %0.02f deg", id, angle_deg);

    l3xz::driver::MX28::AngleDataVect angle_vect_set = angle_vect_act;
    std::transform(angle_vect_act.begin(),
                   angle_vect_act.end(),
                   angle_vect_set.begin(),
                   [](std::tuple<uint8_t, float> const & in) -> std::tuple<uint8_t, float>
                   {
                     auto [id, angle_set] = in;
                     angle_set += 10.0f;
                     while (angle_set > 360.0f)
                      angle_set -= 360.0f;
                     return std::make_tuple(id, angle_set);
                   });

    for (auto [id, angle_deg] : angle_vect_set)
      ROS_INFO("[ID:%03d] Angle Set = %0.02f deg", id, angle_deg);


    if (!mx28_ctrl->setAngle(angle_vect_set))
      ROS_ERROR("setAngle() failed");
*/
  }

  deinit_dynamixel(mx28_ctrl);

  return EXIT_SUCCESS;
}
catch (std::runtime_error const & err)
{
  ROS_ERROR("Exception caught: %s\nTerminating ...", err.what());
  return EXIT_FAILURE;
}

/**************************************************************************************
 * FUNCTION DECLARATION
 **************************************************************************************/

bool init_dynamixel(l3xz::driver::SharedMX28 & mx28_ctrl)
{
  std::optional<l3xz::driver::Dynamixel::IdVect> opt_act_id_vect = mx28_ctrl->discover();

  if (!opt_act_id_vect) {
    ROS_ERROR("Zero MX-28 servos detected.");
    return false;
  }

  std::stringstream act_id_list;
  for (auto id : opt_act_id_vect.value())
    act_id_list << static_cast<int>(id) << " ";
  ROS_INFO("Detected Dynamixel MX-28: { %s}", act_id_list.str().c_str());

  bool all_req_id_found = true;
  for (auto req_id : DYNAMIXEL_ID_VECT)
  {
    bool const req_id_found = std::count(opt_act_id_vect.value().begin(),
                                         opt_act_id_vect.value().end(),
                                         req_id) > 0;
    if (!req_id_found) {
      all_req_id_found = false;
      ROS_ERROR("Unable to detect required dynamixel with node id %d", static_cast<int>(req_id));
    }
  }
  if (!all_req_id_found)
    return false;

  auto isInitialTargetAngleReached = [](l3xz::driver::MX28::AngleDataSet const & current_angle)
  {
    for (auto [actual_id, actual_angle_deg] : current_angle)
    {
      if (!L3XZ_INITIAL_ANGLE_DATA_SET.count(actual_id)) {
        ROS_ERROR("Could not find ID for angle comparison");
        return false;
      }

      float const EPSILON = 1.0f;
      float const set_angle_deg = L3XZ_INITIAL_ANGLE_DATA_SET.at(actual_id);
      float const abs_angle_diff = fabs(actual_angle_deg - set_angle_deg);

      if (abs_angle_diff > EPSILON) {
        ROS_INFO("Not yet target angle reached for ID %d (set: %.2f, actual: %.2f)", actual_id, set_angle_deg, actual_angle_deg);
        return false;
      }
    }
    return true;
  };

  mx28_ctrl->torqueOn(DYNAMIXEL_ID_VECT);

  if (!mx28_ctrl->setAngle(L3XZ_INITIAL_ANGLE_DATA_SET)) {
    ROS_ERROR("failed to set initial angles for all dynamixel servos");
    return false;
  }

  std::this_thread::sleep_for(std::chrono::seconds(1));

  if (!isInitialTargetAngleReached(mx28_ctrl->getAngle(DYNAMIXEL_ID_VECT))) {
    ROS_ERROR("failed to set all dynamixel servos to initial position");
    return false;
  }

  return true;
}

void deinit_dynamixel(l3xz::driver::SharedMX28 & mx28_ctrl)
{
  mx28_ctrl->torqueOff(DYNAMIXEL_ID_VECT);
}

void cmd_vel_callback(const geometry_msgs::Twist::ConstPtr & msg)
{
  float const pitch = msg->angular.x;
  float const yaw   = msg->angular.y;

  l3xz_mx28_target_angle[7] = 180.0f + pitch;
  l3xz_mx28_target_angle[8] = 180.0f + yaw;

  ROS_INFO("pitch = %0.2f, yaw = %.2f", pitch, yaw);
}