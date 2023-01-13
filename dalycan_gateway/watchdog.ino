void watchdog(int watchdogTime)
{
  unsigned long currentMillisWatchdog = millis();
  if (currentMillisWatchdog - previousMillisWatchdog >= watchdogTime)
  {
    previousMillisWatchdog = currentMillisWatchdog;
    client.publish(STATE_WATCHDOG_TOPIC, "true");
  }
}
