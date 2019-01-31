#include <bits/stdc++.h>
#include <hyro/Thresholding.h>

namespace hyro 
{

Thresholding::Thresholding()
  : m_amplitude(1)
  , m_threshold(0)
{
}


double Thresholding::calculateThreshold(double value)
{
  if (value > m_threshold) 
    return m_amplitude; 
  return 0; 
}

void Thresholding::setThreshold(double th) { m_threshold = th; }

double Thresholding::getThreshold() { return m_threshold; }

void Thresholding::setAmplitude(double amp) { m_amplitude = amp; }

double Thresholding::getAmplitude() { return m_amplitude; }

} //namespace hyro