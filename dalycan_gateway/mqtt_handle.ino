void mqttHandle()
{
  if ((!client.connected()) && (eth_connected))
  {
    mqttconnect();
  }
  client.loop();
}

void receivedCallback(char* topic, byte * payload, unsigned int length) {

  Serial.print("Message received: ");
  Serial.println(topic);

  Serial.print("payload: ");

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // SOC
  /*
    if (strcmp((char*)topic, ACT_SOC_TOPIC) == 0)
    {
    payload[length] = '\0';
    String s = String((char*)payload);
    soc = s.toInt();
    //    Serial.println(s);
    //    Serial.println(soc);
    }
  */
  // CHGVLT
  if (strcmp((char*)topic, ACT_CHGVLT_TOPIC) == 0)
  {
    payload[length] = '\0';
    String s = String((char*)payload);
    chgVlt = s.toInt();
  }

  // DISVLT
  if (strcmp((char*)topic, ACT_DISVLT_TOPIC) == 0)
  {
    payload[length] = '\0';
    String s = String((char*)payload);
    disVlt = s.toInt();
  }

  // CHGCUR
  if (strcmp((char*)topic, ACT_CHGCUR_TOPIC) == 0)
  {
    payload[length] = '\0';
    String s = String((char*)payload);
    chgCur = s.toInt();
  }

  // DISCUR
  if (strcmp((char*)topic, ACT_DISCUR_TOPIC) == 0)
  {
    payload[length] = '\0';
    String s = String((char*)payload);
    disCur = s.toInt();
  }
  // ESP Restart
  if (strcmp((char*)topic,  CTRL_RESTART_TOPIC ) == 0)
  {
    if (strncmp((char*)payload, "true", length) == 0)
    {
      ESP.restart();
    }
  }


  // Relais 1
  if (strcmp((char*)topic,  CTRL_O1_TOPIC ) == 0)
  {
    if (strncmp((char*)payload, "true", length) == 0)
    {
      sw(RELAY1, true);
      unsigned long currentMillisRelais1 = millis();
      if ((currentMillisRelais1  - previousMillisRelais1  >= intervalRelais1) && (relay1State == true))
      {
        previousMillisRelais1  = currentMillisRelais1 ;
        sw(RELAY1, false);
      }
    }
    if (strncmp((char*)payload, "false", length) == 0)
    {
      sw(RELAY1, false);
    }
  }

  // Relais 2
  if (strcmp((char*)topic,  CTRL_O2_TOPIC ) == 0)
  {
    if (strncmp((char*)payload, "true", length) == 0)
    {
      sw(RELAY2, true);
    }
    if (strncmp((char*)payload, "false", length) == 0)
    {
      sw(RELAY2, false);
    }
  }

  // Setze Abfragezeit von ESP zu BMS
  if (strcmp((char*)topic, CTRL_REQUEST_TOPIC) == 0)
  {
    ergebnis = 0;
    for (int i = 0; i < length; i++) ergebnis = (ergebnis * 10) + (payload[i] - '0');
    intervalBMS = ergebnis;
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
