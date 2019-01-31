#include <bits/stdc++.h>
#include <hyro/SignalGenerator.h>

namespace hyro 
{

SignalGenerator::SignalGenerator()
  : m_deg(0.0)
  , m_step(1.0)
  , m_amp(1.0)
{
}

double SignalGenerator::getSignalValue()
{
  m_rad = m_deg / 180.0 * M_PI;
  m_deg += m_step;

  if (m_cosine) 
    return m_amp * cos(m_rad);
  return  m_amp * sin(m_rad);
}

// void SignalGenerator::setAmplitude(double amplitude) { m_amp = amplitude; }

// double SignalGenerator::getAmplitude() { return m_amp; }

// void SignalGenerator::setFrequency(double frequency) { m_step = ( 360.0 * frequency * 10.0 ) / 1000.0 ; }

// double SignalGenerator::getFrequency() { return 1000.0 * m_step / ( 360.0 * 10.0 ); }

// void SignalGenerator::setCosine(bool coss) { m_cosine = coss; }

// bool SignalGenerator::getCosine() { return m_cosine; }

}