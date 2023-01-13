#include <WiFi.h>
#include <SoftwareSerial.h>
#include <ArduinoOTA.h>             // OTA Library
#include <PubSubClient.h>           // MQTT Library
#include <ArduinoJson.h>
#include <credentials.h>

#define MQTT_MAX_PACKET_SIZE 2500

SoftwareSerial swSer(27, 25); // Hier Softwareserial Pins (RX, TX)


/*
  ------------------------ Mosquitto --------------------------
*/

const char* mqtt_server = mqttServer;                       // MQTT Server IP

WiFiClient espClient;                                             // WiFi client
PubSubClient client(espClient);                                   // Client für MQTT

// MQTT Topics können angepasst werden
#define ACT_DATA_TOPIC            "Solar/BMS/act/data"
#define STATE_DEBUG_TOPIC         "Solar/BMS/debug/debug"
#define CTRL_DISC_TOPIC           "Solar/BMS/ctrl/disc"
#define CTRL_CHG_TOPIC            "Solar/BMS/ctrl/chg"
#define CTRL_REQUEST_TOPIC        "Solar/BMS/ctrl/request"
#define CTRL_RESTART_TOPIC        "Solar/BMS/ctrl/restart"
#define CTRL_RESBMS_TOPIC         "Solar/BMS/ctrl/restartBMS"
#define STATE_WATCHDOG_TOPIC      "Solar/BMS/state/watchdog"

/*
  ---------------------- WLan Einstellungen ------------------
*/

const char* ssid = wifiName;                             // Wlan Name
const char* password = wifiPass;            // Wlan Passwort

IPAddress ip(192, 168, 10, 107);                   // IP mit der sich der ESP in das Netzwerk verbinden soll
IPAddress gateway(192, 168, 10, 1);                // Gateway des Netzwerks
IPAddress subnet(255, 255, 255, 0);                // Subnetmask
IPAddress dns(192, 168, 10, 1);                    // DNS

/*
  ---------------------- Variablen ------------------
*/

//byte command;

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
bool socFirstRead = true;
bool cellFirstRead = true;
bool socValid = false;

byte data[13];
byte bmsState = 0;
byte minCelNo = 0;
byte maxCelNo = 0;


int vpv;
int cellVmV[17];
int temp1 = 0;
int current = 0;
int ergebnis = 0;
int minCelVlt = 0;
int lastMinCelVlt = 0;
int maxCelVlt = 0;
int lastMaxCelVlt = 0;
int diffCelVlt = 0;
int totVlt1 = 0;
int totVlt2 = 0;
int numberOfCells = 0;
int reqNum = 1;
int counter = 0;

long interval1 = 5000;
unsigned long previousMillis1 = 0;
unsigned long previousMillisWatchdog = 0;
long watchdogTime = 0;


uint32_t capacity = 0;
uint32_t soc = 0;
uint32_t lastSoC = 0;

char checksum = 0;
char checksum2 = 0;
char test[20];

String valueVPV;
String valuePID;
const int BUFFER_SIZE = 13;
char buf[BUFFER_SIZE];

void setup()
{
  Serial.begin(115200);   // Serielle Verbindung zum Debuggen öffnen
  swSer.begin(9600);      // Serielle Verbindung zum BMS Starten
  Serial2.begin(9600);

  wifiSettings();                 // WLAN Konfiguration
  mqttSettings();                 // MQTT Konfigurieren
  OTAsettings();                  // OTA Konfigurieren
  mqttconnect();                  // MQTT-Verbindung aufbauen

  client.publish(STATE_DEBUG_TOPIC, "Setup Done");    // Nachricht an MQTT-Server schicken
}

void loop()
{
  ArduinoOTA.handle();            // Zyklischer OTA aufruf
  mqttHandle();                   // Sende / Empfange Daten über MQTT
  watchdog(5000);
  watchWifi();
  readSer();
  // Abfragen der Daten vom BMS und senden per MQTT im Zyklus von "interval1"

  unsigned long currentMillis1 = millis();
  if (currentMillis1 - previousMillis1 >= interval1)
  {
    previousMillis1 = currentMillis1;
    request(0x90);
    request(0x91);
    request(0x93);
    request(0x94);
    request(0x95);
    request(0x96);
    //  delay(5);
    json1();    // Aufarbeiten der Daten in ein JSON Objekt
  }
}
