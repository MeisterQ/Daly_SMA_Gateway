void mqttHandle()
{
  if ((!client.connected()) && (WiFi.status() == WL_CONNECTED))
  {
    mqttconnect();
  }
  client.loop();
}

void receivedCallback(char* topic, byte * payload, unsigned int length) {

  Serial.print("Message received: ");
  Serial.println(topic);

  Serial.print("payload: ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();

  // Setze Abfragezeit von ESP zu BMS
  if (strcmp((char*)topic, CTRL_REQUEST_TOPIC) == 0)
  {
    ergebnis = 0;
    for (int i = 0; i < length; i++) ergebnis = (ergebnis * 10) + (payload[i] - '0');
    interval1 = ergebnis;
  }


  // Entlade-Mosfets An/Aus
  if (strcmp((char*)topic,  CTRL_DISC_TOPIC ) == 0)
  {
    if (strncmp((char*)payload, "true", length) == 0)
    {
      dischargeOn();
    }
    if (strncmp((char*)payload, "false", length) == 0)
    {
      dischargeOff();
    }
  }
  
  // Lade-Mosfets An/Aus
  if (strcmp((char*)topic,  CTRL_CHG_TOPIC ) == 0)
  {
    if (strncmp((char*)payload, "true", length) == 0)
    {
      chargeOn();
    }
    if (strncmp((char*)payload, "false", length) == 0)
    {
      chargeOff();
    }
  }

  // ESP Restart
  if (strcmp((char*)topic,  CTRL_RESTART_TOPIC ) == 0)
  {
    if (strncmp((char*)payload, "true", length) == 0)
    {
      ESP.restart();
    }
  }

  // BMS Restart 
  if (strcmp((char*)topic,  CTRL_RESBMS_TOPIC ) == 0)
  {
    if (strncmp((char*)payload, "true", length) == 0)
    {
      request(0x00);
    }
  }



}
