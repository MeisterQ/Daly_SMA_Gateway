void json1()
{
  StaticJsonBuffer<350> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();

  JSONencoder["SOC"] = soc;
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
  values.add(cellVmV[0]);
  values.add(cellVmV[1]);
  values.add(cellVmV[2]);
  values.add(cellVmV[3]);
  values.add(cellVmV[4]);
  values.add(cellVmV[5]);
  values.add(cellVmV[6]);
  values.add(cellVmV[7]);
  values.add(cellVmV[8]);
  values.add(cellVmV[9]);
  values.add(cellVmV[10]);
  values.add(cellVmV[11]);
  values.add(cellVmV[12]);
  values.add(cellVmV[13]);
  values.add(cellVmV[14]);
  values.add(cellVmV[15]);

  char JSONmessageBuffer2[350];
  JSONencoder2.printTo(JSONmessageBuffer2, sizeof(JSONmessageBuffer2));
  client.publish("Solar/BMS/cellvoltages", JSONmessageBuffer2);

}
