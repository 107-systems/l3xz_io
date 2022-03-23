/**
 * Copyright (c) 2022 LXRobotics GmbH.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-UAVCAN/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <l3xz/driver/dynamixel/Dynamixel.h>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace l3xz::driver
{

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

Dynamixel::Dynamixel(std::string const device_name,
                     float       const protocol_version,
                     int         const baudrate)
: _port_handler{dynamixel::PortHandler::getPortHandler(device_name.c_str())}
, _packet_handler{dynamixel::PacketHandler::getPacketHandler(protocol_version)}
{
  if (!_port_handler->openPort())
    printf("%s::%s error, 'PortHandler::openPort()' failed.", __FILE__, __FUNCTION__);

  if (!_port_handler->setBaudRate(baudrate))
    printf("%s::%s error, 'PortHandler::setBaudRate()' failed.", __FILE__, __FUNCTION__);
}

Dynamixel::~Dynamixel()
{
  _port_handler->closePort();
}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTION
 **************************************************************************************/

std::tuple<Dynamixel::Error, Dynamixel::IdVect> Dynamixel::broadcastPing()
{
  IdVect servo_id_vect;

  if (int const res = _packet_handler->broadcastPing(_port_handler.get(), servo_id_vect); res != COMM_SUCCESS)
  {
    printf("%s::%s error, 'PacketHandler::broadcastPing()' failed.", __FILE__, __FUNCTION__);
    return std::make_tuple(Error::BroadcastPing, servo_id_vect);
  }

  return std::make_tuple(Error::None, servo_id_vect);
}

Dynamixel::Error Dynamixel::syncWrite(uint16_t const start_address, uint16_t const data_length, SyncWriteData const & data)
{
  return syncWrite(start_address, data_length, std::vector<SyncWriteData>{data});
}

Dynamixel::Error Dynamixel::syncWrite(uint16_t const start_address, uint16_t const data_length, SyncWriteDataVect const & data)
{
  dynamixel::GroupSyncWrite group_sync_write(_port_handler.get(), _packet_handler.get(), start_address, data_length);

  for(auto [id, data_ptr] : data)
  {
    if (!group_sync_write.addParam(id, data_ptr))
      return Error::AddParam;
  }

  if (int res = group_sync_write.txPacket(); res != COMM_SUCCESS)
  {
    printf("%s::%s error, 'GroupSyncWrite::txPacket()' %s", __FILE__, __FUNCTION__, _packet_handler->getTxRxResult(res));
    return Error::TxPacket;
  }

  group_sync_write.clearParam();

  return Error::None;
}

std::tuple<Dynamixel::Error, Dynamixel::SyncReadData> Dynamixel::syncRead(uint16_t const start_address, uint16_t const data_length, uint8_t const id)
{
  auto [err, data] = syncRead(start_address, data_length, std::vector<uint8_t>{id});
  return std::make_tuple(err, data.at(0));
}

std::tuple<Dynamixel::Error, Dynamixel::SyncReadDataVect> Dynamixel::syncRead(uint16_t const start_address, uint16_t const data_length, IdVect const & id_vect)
{
  SyncReadDataVect data_vect;

  dynamixel::GroupSyncRead group_sync_read(_port_handler.get(), _packet_handler.get(), start_address, data_length);

  for(auto id : id_vect)
  {
    if (!group_sync_read.addParam(id))
      return std::make_tuple(Error::AddParam, data_vect);
  }

  if (int res = group_sync_read.txRxPacket(); res != COMM_SUCCESS)
  {
    printf("%s::%s error, 'GroupSyncRead::txRxPacket()' %s", __FILE__, __FUNCTION__, _packet_handler->getTxRxResult(res));
    return std::make_tuple(Error::TxRxPacket, data_vect);
  }

  for(auto id : id_vect)
  {
    uint8_t dxl_error = 0;
    if (group_sync_read.getError(id, &dxl_error))
      printf("%s::%s 'GroupSyncRead::getError(%d)' returns %s", __FILE__, __FUNCTION__, id, _packet_handler->getRxPacketError(dxl_error));
  }

  for(auto id : id_vect)
  {
    if (group_sync_read.isAvailable(id, start_address, data_length))
      data_vect.push_back(std::make_tuple(id, group_sync_read.getData(id, start_address, data_length)));
    else
      data_vect.push_back(std::make_tuple(id, std::nullopt));
  }

  group_sync_read.clearParam();

  return std::make_tuple(Error::None, data_vect);
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* l3xz::driver */