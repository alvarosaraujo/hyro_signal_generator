#include <bits/stdc++.h>
#include <hyro/SignalGenerator.h>

namespace hyro 
{

SignalGenerator::SignalGenerator()
{
  m_deg = 0.0;
  m_step = 1.0;
  m_amp = 1.0; 
}

double SignalGenerator::getSignalValue()
{
  rad = m_deg / 180.0 * M_PI;
  m_deg += m_step;
  
  if (cosine) 
    return m_amp * cos(rad);
  return  m_amp * sin(rad);
}

void SignalGenerator::setAmplitude(double amplitude) { m_amp = amplitude; }

double SignalGenerator::getAmplitude() { return m_amp; }

void SignalGenerator::setFrequency(double frequency) { m_step = ( 360 * frequency * 10 ) / 1000 ; }

double SignalGenerator::getFrequency() { return 1000 * m_step / ( 360 * 10 ); }

void SignalGenerator::setCosine(bool coss) { cosine = coss; }

bool SignalGenerator::getCosine() { return cosine; }

}