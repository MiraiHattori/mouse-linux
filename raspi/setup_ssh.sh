#!/bin/bash

if [ -z "$(grep publish-workstation=yes /etc/avahi/avahi-daemon.conf)" ]; then
    sed -i -e 's/publish-workstation=no/publish-workstation=yes/g' /etc/avahi/avahi-daemon.conf
    service avahi-daemon restart
fi
