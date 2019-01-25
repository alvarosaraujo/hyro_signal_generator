#include <bits/stdc++.h>
#include <hyro/Thresholding.h>

namespace hyro 
{

Thresholding::Thresholding()
{
  amplitude = 1;
  threshold = 0;
}


double Thresholding::get_threshold(double value)
{
  if (value > threshold)
  { return amplitude; }

  else
  { return 0; }
}

void Thresholding::setThreshold(double th) { threshold = th; }

double Thresholding::getThreshold() { return threshold; }

void Thresholding::setAmplitude(double amp) { amplitude = amp; }

double Thresholding::getAmplitude() { return amplitude; }

} //namespace hyro