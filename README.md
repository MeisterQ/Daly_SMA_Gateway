# Daly CAN Gateway

A gateway between BMS UART and SMA Sunny Island CAN

ESP32 based board


Preparation:

You need a ESP32 with CAN transceiver and some free GPIOs

Iam using Olimex ESP32 EVB board which contains a CAN transceiver already and connects via Ethernet.

Every ESP32 board will do fine but will afford abit more work like soldering and you probably need to change abit of code to use it with wifi.


Clone or download all files and go to folder.

Open dalycan_gateway.ino file.

### MQTT

Edit your MQTT Server and Port in Mosquitto section of code and aswell your wanted MQTT topics (Or leave it like it is)

```
/*
  ------------------------ Mosquitto --------------------------
*/

#define mqttPort 1883
const char* mqtt_server = "mqttServerIP";
```

If you want to use an MQTT User and Password, then replace in mqtt.ino following line
```
if (client.connect(clientId.c_str()))
```

with

```
if (client.connect(clientId.c_str(), "User", "Password"))
```

----------------------------------


### Network

Edit your IP Adresses in section Network

```
/*
  ---------------------- Network ------------------
*/


IPAddress ip(192, 168, 10, 51);       // IP
IPAddress gateway(192, 168, 10, 1);   // Gateway
IPAddress subnet(255, 255, 255, 0);   // Subnet
IPAddress dns(192, 168, 10, 1);       // DNS
```

### CAN

Edit your CAN RX and TX pins here

```
/*
  ---------------------- CAN ----------------------------
*/

#define CAN_H GPIO_NUM_5
#define CAN_L GPIO_NUM_35

``` 

------------------------------------

### Software Serial

Software Serial Pins can be changed here

```
#define RXPin 13
#define TXPin 16
```

-------------------------------------

Comment line 
```
#include <credentials.h>
```

if you are not using a credentials file for your Wifi, MQTT etc...


-------------------------------------

### Wiring

Connect CAN H, CAN L, GND of Sunny Island CAN Pins and GPIO 27 (RX) and GPIO 25 (TX) with the BMS UART Pins crosswise.

Upload sketch to the board.

More information following soon.