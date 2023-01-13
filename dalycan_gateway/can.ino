void canConfig()
{
  CAN_cfg.speed = CAN_SPEED_500KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_5;
  CAN_cfg.rx_pin_id = GPIO_NUM_35;
  CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
  ESP32Can.CANInit();

}


void can()
{
  unsigned long currentMillisCAN = millis();
  if ((currentMillisCAN  - previousMillisCAN  >= intervalCAN ))
  {
    previousMillisCAN  = currentMillisCAN ;

    sohLow =  soh & 0xff; // Nur die unteren 8 Bit
    sohHigh = soh >> 8; // um 8 Bit nach rechts schieben

    socLow =  soc & 0xff; // Nur die unteren 8 Bit
    socHigh = soc >> 8; // um 8 Bit nach rechts schieben

    chgVltLow =  chgVlt & 0xff; // Nur die unteren 8 Bit
    chgVltHigh = chgVlt >> 8; // um 8 Bit nach rechts schieben

    chgCurLow =  chgCur & 0xff; // Nur die unteren 8 Bit
    chgCurHigh = chgCur >> 8; // um 8 Bit nach rechts schieben

    disCurLow =  disCur & 0xff; // Nur die unteren 8 Bit
    disCurHigh = disCur >> 8; // um 8 Bit nach rechts schieben

    disVltLow =  disVlt & 0xff; // Nur die unteren 8 Bit
    disVltHigh = disVlt >> 8; // um 8 Bit nach rechts schieben

    data351[0] = chgVltLow;
    data351[1] = chgVltHigh;
    data351[2] = chgCurLow;
    data351[3] = chgCurHigh;
    data351[4] = disCurLow;
    data351[5] = disCurHigh;
    data351[6] = disVltLow;
    data351[7] = disVltHigh;

    data355[0] = socLow;
    data355[1] = socHigh;
    data355[2] = sohLow;
    data355[3] = sohHigh;

    ESP32Can.CANInit();
    CAN_frame_t tx_frame;
    tx_frame.FIR.B.FF = CAN_frame_std;

    if (canID == 0)
    {
      Serial.println("Sending package 351: ");
      tx_frame.MsgID = 0x351;
      tx_frame.FIR.B.DLC = 8;
      tx_frame.data.u8[0] = chgVltLow;
      tx_frame.data.u8[1] = chgVltHigh;
      tx_frame.data.u8[2] = chgCurLow;
      tx_frame.data.u8[3] = chgCurHigh;
      tx_frame.data.u8[4] = disCurLow;
      tx_frame.data.u8[5] = disCurHigh;
      tx_frame.data.u8[6] = disVltLow;
      tx_frame.data.u8[7] = disVltHigh;
      int sent351 = ESP32Can.CANWriteFrame(&tx_frame);

      if (sent351 == 0)
      {
        Serial.println("Package 351 sent successfully");
        client.publish(STATE_DEBUG_TOPIC, "Package 351 sent successfully");
      }
      else
      {
        Serial.println("Failed to send");
        client.publish(STATE_DEBUG_TOPIC, "Failed to CAN");
      }
      canID = 1;
    }
    else if (canID == 1)
    {
      Serial.println("Sending package 355: ");
      tx_frame.MsgID = 0x355;
      tx_frame.FIR.B.DLC = 8;
      tx_frame.data.u8[0] = socLow;
      tx_frame.data.u8[1] = socHigh;
      tx_frame.data.u8[2] = sohLow;
      tx_frame.data.u8[3] = sohHigh;
      int sent355 = ESP32Can.CANWriteFrame(&tx_frame);
      if (sent355 == 0)
      {
        Serial.println("Package 355 sent successfully");
        client.publish(STATE_DEBUG_TOPIC, "Package 355 sent successfully");
      }
      else
      {
        Serial.println("Failed to send");
        client.publish(STATE_DEBUG_TOPIC, "Failed to CAN");
      }
      canID = 0;
    }
    ESP32Can.CANStop();
  }
}
