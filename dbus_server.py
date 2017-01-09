#!/usr/bin/env python3

############################################################################
# Copyright 2016-2017 Libelium Comunicaciones Distribuidas S.L.            #
#                                                                          #
# Licensed under the Apache License, Version 2.0 (the "License");          #
# you may not use this file except in compliance with the License.         #
# You may obtain a copy of the License at                                  #
#                                                                          #
#     http://www.apache.org/licenses/LICENSE-2.0                           #
#                                                                          #
# Unless required by applicable law or agreed to in writing, software      #
# distributed under the License is distributed on an "AS IS" BASIS,        #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. #
# See the License for the specific language governing permissions and      #
# limitations under the License.                                           #
############################################################################

#########################################################
#            AGILE DBus Protocol Server                 #
#                                                       #
#    Description: Runs the AGILE DBus Protocol defined  #
#       in the AGILE API for the XBee 802.15.4 and XBee #
#       ZigBee protocols.                               #
#    Author: David Palomares <d.palomares@libelium.com> #
#    Version: 0.2                                       #
#    Date: November 2016                                #
#########################################################

# --- Imports -----------
import sys
from gi.repository import GLib
import dbus
import dbus.service
import dbus.mainloop.glib
from dbus_protocols import dbus_protocol as dbP
from dbus_protocols import dbus_xbee_802_15_4 as xb_802
from dbus_protocols import dbus_xbee_zigbee as xb_zb
from dbus_protocols import dbus_lorawan as lorawan
from dbus_protocols import dbus_constants as db_cons
import logging
# -----------------------


# --- Variables ---------
LOGLEVEL = logging.INFO # DEBUG, INFO, WARNING, ERROR, CRITICAL
mainloop = GLib.MainLoop()
# -----------------------


# --- Classes -----------
class DBusExit(dbus.service.Object):
    
   def __init__(self):
      super().__init__(dbus.SessionBus(), db_cons.OBJ_PATH) 
    
   @dbus.service.method(db_cons.BUS_NAME, in_signature="", out_signature="")
   def Exit(self):
      mainloop.quit() 
# -----------------------


# --- Functions ---------
def dbusService():
   dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
   dbe = DBusExit()
   xb1 = xb_802.XBee_802_15_4()
   xb2 = xb_zb.XBee_ZigBee()
   lw = lorawan.LoRaWAN()
   logger.info("Running DBus service.")
   try:
      mainloop.run()
   except KeyboardInterrupt:
      print()
      try:
         mainloop.quit()
      except dbus.exceptions.DBusException:
         pass
      endProgram(0)
   
def endProgram(status):
   logger.info("DBus service stopped.")
   sys.exit(status)
# -----------------------
   

# --- Main program ------
if __name__ == "__main__":
   # Start logging
   logging.basicConfig(
      filemode="a",
      format="%(asctime)s [%(levelname)s] %(message)s",
      datefmt="%Y-%m-%d %H:%M:%S",
      level=LOGLEVEL
   )
   logger = logging.getLogger(db_cons.BUS_NAME)
   # Start DBus
   dbusService()   
   endProgram(0)
# -----------------------

