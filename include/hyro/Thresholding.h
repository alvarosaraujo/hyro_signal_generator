#pragma once

#ifndef SIGNAL_GENERATOR_H
#define SIGNAL_GENERATOR_H

#include <bits/stdc++.h>

namespace hyro
{

class Thresholding 
{
  private:

    double amplitude, threshold;

  public:

    Thresholding();
    double get_threshold(double value);
    double getThreshold();
    void setThreshold(double th);
    void setAmplitude(double amp);
    double getAmplitude();
};

}
#endif