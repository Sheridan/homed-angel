#!/bin/bash

if [ ! -f /etc/systemd/system/homed-angel.service ]; then
  cp /usr/share/homed-angel/homed-angel.service /etc/systemd/system/
  systemctl daemon-reload
fi
