
# AGILE DBus Protocol API (iot.agile.Protocol)

This repository contains a Python3 DBus server for the AGILE Protocol API. The server is in **alpha version**, as the AGILE API is being defined.


## Protocols and methods

The server implements the following protocols:
- XBee 802.15.4
- XBee ZigBee
- LoRaWAN/LoRa (Microchip RN2483)

Each protocol might implement the following methods:
- Connected () -> string
- Driver () -> string
- Name () -> string
- Connect () -> void
- Disconnect () -> void
- Discover (a{sv}) -> void
- Exec (sa{sv}) -> void
- Setup (a{sv}) -> void
- Send (a{sv}) -> void
- Receive () -> a{sv}
- Subscribe (a{sv}) -> void


## Installation

In order to run the application, some DBus libraries must be installed in the system.
```
sudo apt-get install libdbus-1-dev libdbus-glib-1-dev python3-gi
```

The python modules required are listed in the `requirements.txt` file, and can be installed from there.
```
sudo python3 -m pip install -r requirements.txt
```


## Usage

The order of execution for any module is: 
- Setup
- Connect
- Send / Receive
- Disconnect. 

The Setup method only applies when the Connect method is called. If no setup parameters are defined, the Conect method may use default parameters.


## Running and exiting the server

To run the server, execute the `dbus_server.py` program. You can set it output log level by changing the variable `LOGLEVEL` any of the levels of the Logging facility for Python. 

There are two ways of exiting the server, either by calling the Exit method (prefered) or by using `Control+C`.
```
dbus-send --session --type=method_call --dest='iot.agile.Protocol' '/iot/agile/Protocol' iot.agile.Protocol.Exit
```

### XBee 802.15.4 and XBee ZigBee modules

##### Setup method

The Setup method will define the parameters that will be applied to the module. The method accepts the type a{sv} (array of "string: variable" pairs, ie. Python's Dictionary).

These "string: variable" pairs can be:
- "baudrate": int -> Defines a valid baudrate for the module. If omitted, defaults to 9600.
- "apiMode2": boolean -> Defines if the module is in API Mode 2. Iif omitted, defaults to false.
- string atCommand1: string -> Two char string defining the AT command to send, the value must be the string representation of the hex parameter (example: {"ID": "A1B2"}).
- string atCommand2: string
- ...

##### Connect method

The Connect method opens the communication with the XBee module and applies the parameters stored in the setup.

##### Send method

The Send method accepts the type a{sv} in order to send information through the XBee module.

The "string: variable" pairs must be:
- "api_command": string -> One of the API Commands of the  API Commands table.
- string field: byte[] -> The fields required by the API Command, whose value must be an array of bytes
- string field2: byte[]
- ...

| API Command | Fields | XBee 802.15.4 | XBee ZigBee |
| ----------- | ------ | ------------- | ----------- |
| at | frame_id, command, parameter | **✔** | **✔** |
| queued_at | frame_id, command, parameter | **✔** | **✔** |
| remote_at | frame_id, dest_addr_long, dest_addr, options, command, parameter | **✔** | **✔** |
| tx_long_addr | frame_id, dest_addr, options, data | **✔** | **✗** |
| tx | frame_id, dest_addr, options, data | **✔** | **✔** |
| tx_explicit | frame_id, dest_addr_long, dest_addr, src_endpoint, cluster, profile, broadcast_radius, optios, data | **✗** | **✔** |

##### Receive method

The Receive method returns a frame received by the module in the format of a{sv}. The fields of the frame depend on the response and can be check in the API Responses table.

| API Responses | Fields | XBee 802.15.4 | XBee ZigBee |
| ------------- | ------ | ------------- | ----------- |
| (0x80) rx_long_addr | source_addr, rssi, options, rf_data | **✔** | **✗** |
| (0x81) rx | source_addr, rssi, options, rf_data | **✔** | **✗** |
| (0x82) rx_io_data_long_addr | source_addr_long, rssi, options, samples | **✔** | **✗** |
| (0x83) rx_io_data | source_addr, rssi, options, samples | **✔** | **✗** |
| (0x88) at_response | frame_id, command, status, parameter | **✔** | **✔** |
| (0x89) tx_status | frame_id, status | **✔** | **✗** |
| (0x8A) status | status | **✔** | **✔** |
| (0x8B) tx_status | frame_id, dest_addr, retries, deliver_status, discover_status | **✗** | **✔** |
| (0x90) rx | source_addr_long, source_addr, options, rf_data | **✗** | **✔** |
| (0x91) rx_explicit | source_addr_long, source_addr, source_endpoint, dest_endpoint, cluster, profile, options, rf_data | **✗** | **✔** |
| (0x92) rx_io_data_long_addr | source_addr_long, source_addr, options, samples | **✗** | **✔** |
| (0x95) node_id_indicator | sender_addr_long, sender_addr, options, source_addr, source_addr_long, node_id, parent_source_addr, device_type, source_event, digi_profile_id, manufacturer_id | **✗** | **✔** |
| (0x97) remote_at_response | frame_id, source_addr_long, source_addr, command, status, parameter | **✔** | **✔** |


##### Disconnect method

The Disconnect method closes the communication with the XBee module.


### LoRAWAN/LoRA module

##### Setup method

The Setup method will define the parameters that will be applied to the module. The method accepts the type a{sv} (array of "string: variable" pairs, ie. Python's Dictionary).

These "string: variable" pairs can be:
- "baudrate": int -> Defines a valid baudrate for the module. If omitted, defaults to 57600.
- "save": boolean -> Defines if the LoRaWAN parameters will be saved in the module's EEPROM. The LoRa parameters cannot be saved. If omitted, defaults to false.
- "mode": string -> Defines the mode that the module will be use, posible values are "LoRaWAN" and "LoRa". If omitted, defaults to "LoRaWAN".
- "join": string -> If the "mode" is "LoRaWAN", this parameter defines how the module will join to the network: "OTAA" or "ABP". If omitted, defaults to "OTAA".

The rest of the setup parameters depends on the "mode" selected.

| Mode | LoRaWAN (OTAA) | LoRaWAN (ABP) | LoRa
| ---- | -------------- | ------------- | ----
| deveui | **✔** | **Optional** | **✗** |
| appeui | **✔** | **✗** | **✗** |
| appkey | **✔** | **✗** | **✗** |
| devaddr | **✗** | **✔** | **✗** |
| nwkskey | **✗** | **✔** | **✗** |
| appskey | **✗** | **✔** | **✗** |
| freq | **✗** | **✗** | **✔** |
| sf | **✗** | **✗** | **✔** |
| cr | **✗** | **✗** | **✔** |
| bw | **✗** | **✗** | **✔** |
| crc | **✗** | **✗** | **✔** |
| pwr | **✗** | **✗** | **✔** |

These parameters are:
- "deveui": string -> Defines the Device EUI (8-byte hexadecimal number). If omitted, defaults to the parameter saved in the module's EEPROM.
- "appeui": string -> Defines the Application EUI (8-byte hexadecimal number). If omitted, defaults to the parameter saved in the module's EEPROM.
- "appkey": string -> Defines the Application Key (16-byte hexadecimal number). If omitted, defaults to the parameter saved in the module's EEPROM.
- "devaddr": string -> Defines the Device Address (4-byte hexadecimal number). If omitted, defaults to the parameter saved in the module's EEPROM.
- "nwkskey": string -> Defines the Network Session Key (16-byte hexadecimal number). If omitted, defaults to the parameter saved in the module's EEPROM.
- "appskey": string -> Defines the Application Session Key (16-byte hexadecimal number). If omitted, defaults to the parameter saved in the module's EEPROM.
- "freq": string -> Defines the Frequency value. Valid values go from "433050000" to "434790000" and from "863000000" to "870000000", in Hz. If omitted, defaults to "868100000".
- "sf": string -> Defines the Spreading Factor value. The valid values are "sf7", "sf8", "sf9", "sf10", "sf11" or "sf12". If omitted, defaults to "sf12".
- "cr": string -> Defines the Coding Rate value. The valid values are "4/5", "4/6", "4/7", "4/8". If omitted, defaults to "4/5".
- "bw": string -> Defines the Bandwidth value. The valid values are "125", "250" or "500", in kHz. If omitted, defaults to "125".
- "crc": string -> Defines the use of CRC. Valid values are "on" or "off". If omitted, defaults to "on".
- "pwr": string -> Defines the power of the module in dBm. The valid values go from "-3" to "15", although the maximum by design is "14". If omitted, defaults to "13".

Example of the setup a{sv}: `{'baudrate': 57600, 'save': True, 'mode': 'LoRaWAN', 'join': 'OTAA', 'deveui': '0102030405060708', 'appeui': '0102030405060708', 'appskey': '0102030405060708090A0B0C0D0E0F00'}`


##### Connect method

The Connect method opens the communication with the LoRaWAN module and applies the parameters stored in the setup.

##### Send method

The Send method accepts the type a{sv} in order to send information through the LoRaWAN module.

The "string: variable" pairs must be:
- "type": string -> "LoRaWAN" mode only. The type of the frame to send: "uncnf" (unconfirmed, no ACK) or "cnf" (confirmed, with ACK). If omitted, defaults to "uncnf".
- "port": int -> "LoRaWAN" mode only. The port to use. If omitted, defaults to 3.
- "data": string -> An hexadecimal representation of the data.


##### Receive method

Only the "LoRa" mode can receive data. The Receive method returns a frame received by the module in the format of a{sv}. The only field of the frame is "data", and its content is the hexadecimal representation of the data.


##### Disconnect method

The Disconnect method closes the communication with the LoRaWAN module.

