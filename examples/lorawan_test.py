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
#                    LoRaWAN Test                       #
#                                                       #
#    Description: Test the AGILE LoRaWAN module with    #
#       the DBus AGILE Protocol API.                    #
#    Author: David Palomares <d.palomares@libelium.com> #
#    Version: 1.0                                       #
#    Date: November 2016                                #
#########################################################

# --- Imports -----------
import sys
import signal
import time
import RPi.GPIO as GPIO
import dbus
# -----------------------


# --- Variables ---------
#GPIOs
PINPOWER = 16 # Must be HIGH for shield to work
# DBus
BUS_NAME = "iot.agile.Protocol"
OBJ_PATH = "/iot/agile/Protocol"
SOCKET0 = "socket0"
SOCKET1 = "socket1"
LORAWAN = "LoRaWAN"
lw = None
# LoRaWAN
setup_lorawan = {
   "baudrate": 57600,
   "mode": "LoRaWAN",
   "save": False,
   "join": "OTAA",
   "deveui": "0102030405060708",
   "appeui": "0102030405060708",
   "appkey": "0102030405060708090A0B0C0D0E0F00"
   #"devaddr": "01020304",
   #"nwkskey": "0102030405060708090A0B0C0D0E0F00",
   #"appskey": "0102030405060708090A0B0C0D0E0F00"
}
send_lorawan = {
   "type": "uncnf",
   "port": 3,
   "data": "4C6F526157414E20444275732074657374" # "LoRaWAN DBus test" in HEX
}
# LoRa
setup_lora = {
   "baudrate": 57600,
   "mode": "LoRa",
   "freq": "868100000",
   "sf": "sf12",
   "cr": "4/5",
   "bw": "125",
   "crc": "on"
}
send_lora = {
   "data": "4C6F526120444275732074657374" # "LoRa DBus test" in HEX
}
# -----------------------


# --- Functions ---------
def test_lorawan():
   """
   Connect the module and send a message to a base station.
   """
   print("*** Testing LoRaWAN ***")
   try:
      print("Setting the device parameters... ", end="")
      sys.stdout.flush()
      lw.Setup(dbus.Dictionary(setup_lorawan, signature="sv"))
      print("OK")
      try:
         print("Connecting the module... ", end="")
         sys.stdout.flush()
         lw.Connect()
         print("OK")
         try:
            print("Sending the message... ", end="")
            sys.stdout.flush()
            lw.Send(dbus.Dictionary(send_lorawan, signature="sv"))
            print("OK")
         except Exception as err:
            print("Error\n{}".format(err))
         try:
            print("Disconnecting the module... ", end="")
            sys.stdout.flush()
            lw.Disconnect()
            print("OK")
         except Exception as err:
            print("Error\n{}".format(err))
      except Exception as err:
         print("Error\n{}".format(err))
   except Exception as err:
      print("Error\n{}".format(err))

def test_lora():
   """
   Connect the module and send and receive a message point to point.
   """
   print("\n*** Testing LoRa ***")
   try:
      print("Setting the device parameters... ", end="")
      sys.stdout.flush()
      lw.Setup(dbus.Dictionary(setup_lora, signature="sv"))
      print("OK")
      try:
         print("Connecting the module... ", end="")
         sys.stdout.flush()
         lw.Connect()
         print("OK")
         try:
            print("Sending the message... ", end="")
            sys.stdout.flush()
            lw.Send(dbus.Dictionary(send_lora, signature="sv"))
            print("OK")
         except Exception as err:
            print("Error\n{}".format(err))
         try:
            print("(Send a message with another module. The timeout is 15 seconds.)")
            print("Receiving the message... ", end="")
            sys.stdout.flush()
            msg = lw.Receive()
            print("OK")
            if msg:              
               print("Message received: {}".format(msg["data"]))
            else:
               print("No message received")
         except Exception as err:
            print("Error\n{}".format(err))
         try:
            print("Disconnecting the module... ", end="")
            sys.stdout.flush()
            lw.Disconnect()
            print("OK")
         except Exception as err:
            print("Error\n{}".format(err))
      except Exception as err:
         print("Error\n{}".format(err))
   except Exception as err:
      print("Error\n{}".format(err))

def setup():
   """
   Sets the default parameters of the program.
   """
   global lw
   # Signal handler (Ctrl+C exit)
   signal.signal(signal.SIGINT, signal_handler) 
   # GPIOs
   GPIO.setmode(GPIO.BOARD)
   GPIO.setwarnings(False) 
   GPIO.setup(PINPOWER, GPIO.OUT)
   GPIO.output(PINPOWER, GPIO.HIGH)
   # DBus
   session_bus = dbus.SessionBus()
   objLW = session_bus.get_object(BUS_NAME, OBJ_PATH + "/" + LORAWAN + "/" + SOCKET0)
   lw = dbus.Interface(objLW, dbus_interface=BUS_NAME)
   
def signal_handler(signal, frame):
   """
   Handles the SIGINT signal.
   """
   print()
   try:
      print("Disconnecting the module... ", end="")
      lw.Disconnect()
      print("OK")
   except Exception as err:
      print("Error\n{}").format(err)
   endProgram(0)
   
def endProgram(status):
   """
   Exists the program.
   """
   GPIO.output(PINPOWER, GPIO.LOW)
   GPIO.cleanup()
   sys.exit(status)
# -----------------------
   

# --- Main program ------
if __name__ == "__main__":

   # Setup
   setup()
   
   # Test LoRaWAN
   test_lorawan()
   
   # Test LoRa
   test_lora()
   
   endProgram(0)
# -----------------------

