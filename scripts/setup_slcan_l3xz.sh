#!/bin/bash
cd $(dirname $(readlink -f "$0"))
sudo ./setup_slcan.sh --remove-all --basename can --speed-code 5 \
  /dev/serial/by-id/usb-Zubax_Robotics_Zubax_Babel_2300390005514E433734382000000000-if00 \
  /dev/serial/by-id/usb-Zubax_Robotics_Zubax_Babel_26003A000B5150423339302000000000-if00
