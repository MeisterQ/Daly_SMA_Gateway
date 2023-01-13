// A CAN UART Gateway between Daly Smart BMS and SMA Sunny Island battery Inverter
// Created by MeisterQ
// V00.01.01


#include <ETH.h>
#include <ArduinoOTA.h>             // OTA Library
#include <PubSubClient.h>           // MQTT Library
#include <ArduinoJson.h>
#include <ESP32CAN.h>
#include <CAN_config.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
//#include <CAN.h>

#define MQTT_MAX_PACKET_SIZE 2500

// Software Serial
#define RXPin 13
#define TXPin 16
SoftwareSerial swSer(RXPin, TXPin); // Hier Softwareserial Pins (RX, TX)

/*
  ---------------------- Variablen BMS ------------------
*/
byte message[] = { 0xA5, 0x40, 0x93, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80 };

byte discOff[] = { 0xD2, 0x6, 0x0, 0xA6, 0x0, 0x0, 0x7A, 0x4A };
byte discOn[] = { 0xD2, 0x6, 0x0, 0xA6, 0x0, 0x1, 0xBB, 0x8A };

byte chgOff[] = { 0xD2, 0x6, 0x0, 0xA5, 0x0, 0x0, 0x8A, 0x4A };
byte chgOn[] = { 0xD2, 0x6, 0x0, 0xA5, 0x0, 0x1, 0x4B, 0x8A };

// Charge FET on:  D2 6 0 A5 0 1 4B 8A
// Charge FET off: D2 6 0 A5 0 0 8A 4A

// Discharge FET on : D2 6 0 A6 0 1 BB 8A
// Discharge FET off: D2 6 0 A6 0 0 7A 4A

bool chgFetState = 0;
bool discFetState = 0;
bool chgState = 0;
bool loadState = 0;
bool withoutWifi = false;


byte data[13];
byte bmsState = 0;
byte minCelNo = 0;
byte maxCelNo = 0;


int vpv;
int cellVmV[17];
int cellVmVlast[17];
int temp1 = 0;
int current = 0;
int ergebnis = 0;
int minCelVlt = 0;
int maxCelVlt = 0;
int diffCelVlt = 0;
int totVlt1 = 0;
int totVlt2 = 0;
int numberOfCells = 0;

long intervalBMS = 5000;
unsigned long previousMillisBMS = 0;
unsigned long previousMillisWatchdog = 0;
long watchdogTime = 0;

const long intervalRelais1 = 1000;
unsigned long previousMillisRelais1 = 0;


uint32_t capacity = 0;
//uint32_t soc = 0;
uint32_t lastSoC = 0;

char checksum = 0;
char checksum2 = 0;
char test[20];

String valueVPV;
String valuePID;
const int BUFFER_SIZE = 13;
char buf[BUFFER_SIZE];

/*
  ------------------------ Mosquitto --------------------------
*/

const char* mqtt_server = "192.168.10.173";                        // MQTT Server IP

WiFiClient espClient;                                             // WiFi client
PubSubClient client(espClient);                                   // Client für MQTT

/*
  ------------------------ MQTT Topics --------------------------
*/

// Topics CAN
//#define ACT_SOC_TOPIC               "PV/CAN/act/soc"
#define ACT_CHGVLT_TOPIC              "PV/CAN/act/chgvlt"
#define ACT_CHGCUR_TOPIC              "PV/CAN/act/chgcur"
#define ACT_DISVLT_TOPIC              "PV/CAN/act/disvlt"
#define ACT_DISCUR_TOPIC              "PV/CAN/act/discur"

#define CTRL_O1_TOPIC                 "ETH/ctrl/o1/sw"
#define CTRL_O2_TOPIC                 "ETH/ctrl/o2/sw"
#define STATE_O1_TOPIC                "ETH/state/o1/sw"
#define STATE_O2_TOPIC                "ETH/state/o2/sw"

// Topics BMS
#define ACT_DATA_TOPIC                "PV/BMS/act/data"
#define ACT_CELL_TOPIC                "PV/BMS/cellvoltages"
#define STATE_DEBUG_TOPIC             "PV/BMS/debug/debug"
#define CTRL_DISC_TOPIC               "PV/BMS/ctrl/disc"
#define CTRL_CHG_TOPIC                "PV/BMS/ctrl/chg"
#define CTRL_REQUEST_TOPIC            "PV/BMS/ctrl/request"
#define CTRL_RESTART_TOPIC            "PV/BMS/ctrl/restart"
#define CTRL_RESBMS_TOPIC             "PV/BMS/ctrl/restartBMS"
#define STATE_WATCHDOG_TOPIC          "PV/BMS/state/watchdog"

#define RELAY1 32
#define RELAY2 33

bool relay1State = false;
bool relay2State = false;
bool lastRelay1State = true;
bool lastRelay2State = true;
bool socFirstRead = true;
bool cellFirstRead = true;
bool socValid = false;



char relay1StateMSG[2];
char relay2StateMSG[2];


/*
  ---------------------- Netzwerk Einstellungen ------------------
*/


IPAddress ip(192, 168, 10, 51);                   // IP mit der sich der ESP in das Netzwerk verbinden soll
IPAddress gateway(192, 168, 10, 1);                // Gateway des Netzwerks
IPAddress subnet(255, 255, 255, 0);               // Subnetmask
IPAddress dns(192, 168, 10, 1);

/*
  ---------------------- Variablen CAN ----------------------------
*/


CAN_device_t CAN_cfg;               // CAN Config
unsigned long previousMillis = 0;   // will store last time a CAN Message was send
const int interval = 1000;          // interval at which send CAN Messages (milliseconds)
const int rx_queue_size = 10;       // Receive Queue size

int soc = 100;
int socHighRes = 100;
int soh = 100;
int chgVlt = 550;
int chgCur = 1200;
int disVlt = 432;
int disCur = 1200;
int timeoutCnt = 0;
int canID = 0;

byte socLow, socHigh;
byte chgVltLow, chgVltHigh;
byte chgCurLow, chgCurHigh;
byte disCurLow, disCurHigh;
byte disVltLow, disVltHigh;
byte sohLow, sohHigh;

byte data355[8] = {socLow, socHigh, sohLow, sohHigh};
byte data351[8] = {chgVltLow, chgVltHigh, chgCurLow, chgCurHigh, disCurLow, disCurHigh, disVltLow, disVltHigh};
byte data35[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//int ergebnis = 0;
int failCounter = 0;
int waitCounter = 10;

unsigned long previousMillisCAN = 0;
long intervalCAN = 1000;
unsigned long merker = 0;
static bool eth_connected = false;


void setup()
{
  delay(1000);
  Serial.begin(115200);
  swSer.begin(9600);      // Serielle Verbindung zum BMS Starten
  ethSettings();
  canConfig();
  mqttSettings();                 // MQTT Konfigurieren
  OTAsettings();                  // OTA Konfigurieren
  mqttHandle();                  // MQTT-Verbindung aufbauen
  pinState();





  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  client.publish(STATE_DEBUG_TOPIC, "Setup Done");    // Nachricht an MQTT-Server schicken
}

void loop()
{
  ArduinoOTA.handle();            // OTA Handle
  mqttHandle();                   // Sende Daten an MQTT Server
  bms();
  watchdog(5000);
  pinState();
  can();
}
