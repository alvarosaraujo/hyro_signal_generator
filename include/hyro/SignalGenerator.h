#pragma once

#ifndef SIGNAL_GENERATOR_H
#define SIGNAL_GENERATOR_H

#include <bits/stdc++.h>

namespace hyro
{

class SignalGenerator 
{
  private:

    double rad, m_deg, m_step, m_amp;
    bool cosine;
    
  public:

    SignalGenerator();
    //SignalGenerator(double amp, double frequency);

    double getSignalValue();
    void setAmplitude(double amplitude);
    double getAmplitude();
    void setFrequency(double frequency);
    double getFrequency();
    void setCosine(bool cosine);
    bool getCosine();
};

}
#endif