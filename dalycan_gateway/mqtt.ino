void mqttconnect()
{
  while (!client.connected())
  {
    Serial.print("MQTT connecting ...");
    String clientId = "ESP32_GATEWAY";

    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");

      client.subscribe(ACT_CHGVLT_TOPIC);
      client.subscribe(ACT_DISVLT_TOPIC);
      client.subscribe(ACT_CHGCUR_TOPIC);
      client.subscribe(ACT_DISCUR_TOPIC);
      client.subscribe(CTRL_O1_TOPIC);
      client.subscribe(CTRL_O2_TOPIC);
      
      client.subscribe(CTRL_DISC_TOPIC);
      client.subscribe(CTRL_CHG_TOPIC);
      client.subscribe(CTRL_REQUEST_TOPIC);
      client.subscribe(CTRL_RESTART_TOPIC);
      client.subscribe(CTRL_RESBMS_TOPIC);
    }
  }
}
