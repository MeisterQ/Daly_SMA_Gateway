void mqttconnect()
{
  while (!client.connected())
  {
    Serial.print("MQTT connecting ...");
    String clientId = "ESP8266_BMS";

    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");

      client.subscribe(CTRL_DISC_TOPIC);
      client.subscribe(CTRL_CHG_TOPIC);
      client.subscribe(CTRL_REQUEST_TOPIC);
      client.subscribe(CTRL_RESTART_TOPIC);
      client.subscribe(CTRL_RESBMS_TOPIC);
    }
  }
}
