#pragma once

#ifndef SIGNAL_GENERATOR_H
#define SIGNAL_GENERATOR_H

#include <bits/stdc++.h>

namespace hyro
{

class SignalGenerator 
{
  private:

    double m_rad, m_deg, m_step, m_amp;
    bool m_cosine;
    enum function 
    {
      cosine = 0,
      sine = 1
    };
    
  public:

    SignalGenerator();
    SignalGenerator(double amp, double frequency);

    double 
    getSignalValue();

    void 
    setAmplitude(double amplitude) 
    { 
      m_amp = amplitude; 
    }

    double 
    getAmplitude() 
    {
      return m_amp; 
    }

    void 
    setFrequency(double frequency) 
    { 
      m_step = ( 360.0 * frequency * 10.0 ) / 1000.0 ; 
    }

    double 
    getFrequency() 
    { 
      return 1000.0 * m_step / ( 360.0 * 10.0 ); 
    }

    void 
    setCosine(bool coss) 
    { 
      m_cosine = coss; 
    }

    bool 
    getCosine() 
    { 
      return m_cosine; 
    }
};

}
#endif