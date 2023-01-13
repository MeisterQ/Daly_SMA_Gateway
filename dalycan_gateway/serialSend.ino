int request(byte command)
{
  checksum = 0xA5 + 0x40 + command + 0x08 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00;
  checksum &= 0x00FF;
  message[2] = command;
  message[12] = checksum;
  swSer.write(message, sizeof(message));
  readSer();
}



void chargeOn()
{
  swSer.write(chgOn, sizeof(chgOn));
}

void chargeOff()
{
  swSer.write(chgOff, sizeof(chgOff));
}

void dischargeOff()
{
  swSer.write(discOff, sizeof(discOff));
}

void dischargeOn()
{
  swSer.write(discOn, sizeof(discOn));
}
