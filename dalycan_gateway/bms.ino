void bms()
{
  readSer();
  
  unsigned long currentMillisBMS = millis();
  if (currentMillisBMS - previousMillisBMS >= intervalBMS)
  {
    previousMillisBMS = currentMillisBMS;
    request(0x90);
    request(0x91);
    request(0x93);
    request(0x94);
    request(0x95);
    request(0x96);
    json1();
  }
}
