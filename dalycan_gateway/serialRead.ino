void readSer()
{
  if (swSer.available() > 0)
  {
    for (int i = 0; i <= 12; i++)
    {
      data[i] = swSer.read();
      delay(2);
    }
    //  check();
    checkData();
  }
}

void check()
{
  StaticJsonBuffer<350> JSONbuffer3;
  JsonObject& JSONencoder3 = JSONbuffer3.createObject();


  JSONencoder3["data0"] = data[0];
  JSONencoder3["data1"] = data[1];
  JSONencoder3["data2"] = data[2];
  JSONencoder3["data3"] = data[3];
  JSONencoder3["data4"] = data[4];
  JSONencoder3["data5"] = data[5];
  JSONencoder3["data6"] = data[6];
  JSONencoder3["data7"] = data[7];
  JSONencoder3["data8"] = data[8];
  JSONencoder3["data9"] = data[9];
  JSONencoder3["data10"] = data[10];
  JSONencoder3["data11"] = data[11];
  JSONencoder3["data12"] = data[12];

  checksum2 = data[0] + data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7] + data[8] + data[9] + data[10] + data[11];
  JSONencoder3["Checksum"] = lowByte(checksum2);

  if (data[12] == checksum2)
  {
    JSONencoder3["Status"] = true;
  }
  else
  {
    JSONencoder3["Status"] = false;
  }




  char JSONmessageBuffer3[350];
  JSONencoder3.printTo(JSONmessageBuffer3, sizeof(JSONmessageBuffer3));
  client.publish("Solar/BMS/act/test", JSONmessageBuffer3);
}

void checkData()
{
  // SoC
  if (data[2] == 0x90)
  {
    soc = (data[10] << 8) + data[11];
    socHighRes = soc;
    soc = soc / 10;
    current = (data[8] << 8) + data[9];
    totVlt1 = (data[4] << 8) + data[5];
    totVlt2 = (data[6] << 8) + data[7];

    // Spitze nach oben Filtern
    if (socFirstRead)
    {
      socFirstRead = false;
      socValid = true;
    }
    else
    {
      if (soc > 100)
      {
        soc = lastSoC;
        socValid = false;
      }
      else if (soc == 100)
      {
        soc = 100;
        socValid = true;
      }
      else if (soc > lastSoC + 5)
      {
        soc = lastSoC;
        socValid = false;
      }
      else if (soc < lastSoC - 5)
      {
        soc = lastSoC;
        socValid = false;
      }
      else
      {
        soc = soc;
        socValid = true;
      }
    }
    if (socValid == true)
    {
      lastSoC = soc;
    }
  }


  if (data[2] == 0x91)
  {
    minCelNo = data[9];
    maxCelNo = data[6];
    maxCelVlt = (data[4] << 8) + data[5];
    minCelVlt = (data[7] << 8) + data[8];
    diffCelVlt = maxCelVlt - minCelVlt;

  }


  // Mosfet State and BMS State Capacity
  if (data[2] == 0x93)
  {
    bmsState = data[4];
    chgFetState = data[5];
    discFetState = data[6];
    capacity = (data[8] << 24) + (data[9] << 16) + (data[10] << 8) + data[11];
  }

  // Status Information 1
  if (data[2] == 0x94)
  {
    numberOfCells = data[4];
    chgState = data[6];
    loadState = data[7];
  }

  if (data[2] == 0x95)
  {
    if (data[4] == 1)
    {
      cellVmV[0] = (data[5] << 8) + data[6];
      cellVmV[1] = (data[7] << 8) + data[8];
      cellVmV[2] = (data[9] << 8) + data[10];
    }
    if (data[4] == 2)
    {
      cellVmV[3] = (data[5] << 8) + data[6];
      cellVmV[4] = (data[7] << 8) + data[8];
      cellVmV[5] = (data[9] << 8) + data[10];
    }
    if (data[4] == 3)
    {
      cellVmV[6] = (data[5] << 8) + data[6];
      cellVmV[7] = (data[7] << 8) + data[8];
      cellVmV[8] = (data[9] << 8) + data[10];
    }
    if (data[4] == 4)
    {
      cellVmV[9] = (data[5] << 8) + data[6];
      cellVmV[10] = (data[7] << 8) + data[8];
      cellVmV[11] = (data[9] << 8) + data[10];
    }
    if (data[4] == 5)
    {
      cellVmV[12] = (data[5] << 8) + data[6];
      cellVmV[13] = (data[7] << 8) + data[8];
      cellVmV[14] = (data[9] << 8) + data[10];
    }
    if (data[4] == 6)
    {
      cellVmV[15] = (data[5] << 8) + data[6];
      cellVmV[16] = (data[7] << 8) + data[8];
      cellVmV[17] = (data[9] << 8) + data[10];
    }


  }

  // Temperature
  if (data[2] == 0x96)
  {
    if (data[4] == 1)
    {
      temp1 = data[5] - 40;
    }
  }

}
