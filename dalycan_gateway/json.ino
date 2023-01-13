void json1()
{
  StaticJsonBuffer<350> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();

  JSONencoder["SOC"] = socHighRes;
  JSONencoder["I"] = current - 30000;
  JSONencoder["C"] = capacity;
  JSONencoder["V1"] = totVlt1;
  JSONencoder["temp"] = temp1;
  JSONencoder["minCelNo"] = minCelNo;
  JSONencoder["maxCelNo"] = maxCelNo;
  JSONencoder["minCelVlt"] = minCelVlt;
  JSONencoder["maxCelVlt"] = maxCelVlt;
  JSONencoder["diffCelVlt"] = diffCelVlt;
  JSONencoder["STATE"] = bmsState;
  JSONencoder["CHGFET"] = chgFetState;
  JSONencoder["DISFET"] = discFetState;
  JSONencoder["BMSSTATE"] = bmsState;
  JSONencoder["NUMBEROFCELLS"] = numberOfCells;
  JSONencoder["CHGSTATE"] = chgState;
  JSONencoder["LOADSTATE"] = loadState;


  char JSONmessageBuffer[350];
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  client.publish(ACT_DATA_TOPIC, JSONmessageBuffer);


  StaticJsonBuffer<350> JSONbuffer2;
  JsonObject& JSONencoder2 = JSONbuffer2.createObject();

  JsonArray& values = JSONencoder2.createNestedArray("values");
  for (int i = 0; i < 16; i++)
  {
    values.add(sampleAndHold(cellVmV[i], i));
  }
  char JSONmessageBuffer2[350];
  JSONencoder2.printTo(JSONmessageBuffer2, sizeof(JSONmessageBuffer2));
  client.publish(ACT_CELL_TOPIC, JSONmessageBuffer2);

}
