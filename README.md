# Daly CAN Gateway

A gateway between BMS UART and Sunny Island CAN

ESP32 based board


##Preparation:

You need a ESP32 with CAN transceiver and some free GPIOs

Iam using Olimex ESP32 EVB board which contains a CAN transceiver already and connects via Ethernet.

Every ESP32 board will do fine but will afford abit more work like soldering and you probably need to change abit of code to use it with wifi.


Clone or download all files and go to folder.

Open any .ino file to open Arduino IDE.

Edit your MQTT server and MQTT topics

Upload sketch to the board and connect CAN H, CAN L, GND with Sunny Island CAN Pins and GPIO 27 (RX) and GPIO 25 (TX) with the BMS UART Pins crosswise.