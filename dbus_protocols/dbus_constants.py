
############################################################################
# Copyright 2016-2017 Libelium Comunicaciones Distribuidas S.L.            #
#                                                                          #
# Licensed under the Apache License, Version 2.0 (the "License");          #
# you may not use this file except in compliance with the License.         #
# You may obtain a copy of the License at                                  #
#                                                                          #
#    http://www.apache.org/licenses/LICENSE-2.0                            #
#                                                                          #
# Unless required by applicable law or agreed to in writing, software      #
# distributed under the License is distributed on an "AS IS" BASIS,        #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. #
# See the License for the specific language governing permissions and      #
# limitations under the License.                                           #
############################################################################

#########################################################
#                AGILE DBus Constants                   #
#                                                       #
#    Description: Constant variables of the AGILE       #
#       DBus Protocol API.                              #
#    Author: David Palomares <d.palomares@libelium.com> #
#    Version: 0.1                                       #
#    Date: November 2016                                #
#########################################################


# --- Variables ---------
BUS_NAME = "iot.agile.Protocol"
OBJ_PATH = "/iot/agile/Protocol"
SOCKET0 = "socket0"
SOCKET1 = "socket1"
try:
   import RPi.GPIO as GPIO
   if GPIO.RPI_INFO["TYPE"] == "Pi 3 Model B":
      SOCKET0DEV = "/dev/ttyS0"
      SOCKET1DEV = "/dev/ttyS0"
   else:
      SOCKET0DEV = "/dev/ttyAMA0"
      SOCKET1DEV = "/dev/ttyAMA0"
except:
   SOCKET0DEV = "/dev/ttyUSB0"
   SOCKET1DEV = "/dev/ttyUSB1"
SOCKETDEV = {SOCKET0: SOCKET0DEV, SOCKET1: SOCKET1DEV} 
# -----------------------
