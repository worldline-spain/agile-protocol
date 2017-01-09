
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
#            AGILE DBus Protocol ProtocolTemplate       #
#                                                       #
#    Description: Class of the Protocol defined in the  #
#       in the AGILE API with the implementation of the #
#       ProtocolTemplate protocol.                      #
#    Author: David Palomares <d.palomares@libelium.com> #
#    Version: 0.1                                       #
#    Date: June 2016                                    #
#########################################################

# --- Imports -----------
import dbus
import dbus.service
from dbus_protocols import dbus_protocol as dbP
from dbus_protocols import dbus_constants as db_cons
import logging
# -----------------------


# --- Variables ---------
PROTOCOL_NAME = "ProtocolTemplate" #TODO: Search and replace
# -----------------------


# --- Classes -----------
class ProtocolTemplate(dbP.Protocol):
   
   def __init__(self):     
      super().__init__()
      self._protocol_name = PROTOCOL_NAME
      self._exception = ProtocolTemplate_Exception()
      self._objS0 = ProtocolTemplate_Obj(self._socket0)
      self._objS1 = ProtocolTemplate_Obj(self._socket1)
       

class ProtocolTemplate_Exception(dbP.ProtocolException):
   
   def __init__(self, msg=""):
      super().__init__(PROTOCOL_NAME, msg)
      
    
class ProtocolTemplate_Obj(dbP.ProtocolObj):

   def __init__(self, socket):
      super().__init__(PROTOCOL_NAME, socket)
      
   # Override DBus object methods
   #TODO
# -----------------------


