void ethSettings()
{
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  ETH.config(ip, gateway, subnet, dns);
  while ((eth_connected == false) && (timeoutCnt < 30))
  {
    Serial.print(".");
    timeoutCnt++;
    delay(500);
  }
}
