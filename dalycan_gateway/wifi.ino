void wifiSettings()
{
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(500);
  }
}
