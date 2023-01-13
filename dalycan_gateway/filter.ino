int filterVoltage(int zelle, int zelleLast)
{
  if (zelle > (zelleLast + 500))
  {
    zelle = zelleLast;
  }
  else if (zelle < (zelleLast - 500))
  {
    zelle = zelleLast;
  }
  else
  {
    zelle = zelle;
  }

  //return (int)mean;
  return zelle;
}

int sampleAndHold(int zelle, unsigned int ch) {
  static int alterWert[16] = {3300};
  if ((zelle > 3650) || (zelle < 2500)) zelle = alterWert[ch]; else alterWert[ch] = zelle;
  return zelle;
}

int filter(int zelle, int ch)
{
  static float mean[16] = {3300.};
  const float alpha = 0.5;

  if (ch > 15) return -1; // error!

  mean[ch] = alpha * zelle + (1. - alpha) * mean[ch];
  return mean[ch];
}
