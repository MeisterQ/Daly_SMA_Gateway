void pinState()
{
  relay1State = digitalRead(RELAY1);
  relay2State = digitalRead(RELAY2);

  if (relay1State != lastRelay1State)
  {
    snprintf (relay1StateMSG, 2, "%ld", relay1State);
    client.publish(STATE_O1_TOPIC, relay1StateMSG);
    lastRelay1State = relay1State;
  }
  if (relay2State != lastRelay2State)
  {
    snprintf (relay2StateMSG, 2, "%ld", relay2State);
    client.publish(STATE_O2_TOPIC, relay2StateMSG);
    lastRelay2State = relay2State;
  }
}

void sw(int relay, bool state)
{
  digitalWrite(relay, state);
}
