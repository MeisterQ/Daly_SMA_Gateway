void mqttSettings()
{
  client.setServer(mqtt_server, 1883);
  client.setCallback(receivedCallback);
}
