#pragma once

#ifndef HYRO_SIGNALGENERATORCOMPONENT_H
#define HYRO_SIGNALGENERATORCOMPONENT_H

#include <hyro_signal_generator_components_export.h>  

#include <string>
#include <hyro/msgs/Signal.h>
#include <hyro/core/Component.h>
#include <hyro/msgs/common/Basic.h>
#include <hyro/SignalGenerator.h>
#include <hyro/msgs/Signal.h>
#include <hyro/utils/DynamicPropertyAccess.h>
#include <hyro/utils/SpinnerDefault.h>
#include <hyro/factory/CommandFactory.h>

namespace hyro
{

class HYRO_SIGNAL_GENERATOR_COMPONENTS_EXPORT SignalGeneratorComponent : public hyro::Component
{
public:

  using Component::Component;

  inline virtual
  ~SignalGeneratorComponent() override = default;

  // State transitions

  virtual Result
  init (const ComponentConfiguration & configuration) override;

  virtual Result
  start () override;

  virtual Result
  check () override;

  virtual Result
  update () override;

  virtual Result
  reset () override;

  bool 
  setAmplitude(double amplitude);

  double 
  getAmplitude();
  
  bool 
  setFrequency(double frequency);
  
  double 
  getFrequency();

  bool 
  setCosine(int cosine);
  
  int 
  getCosine();

private:

  double m_amplitude, m_frequency;
  int m_cosine;
  SignalGenerator sg;

  static std::shared_ptr<HyroLogger> s_logger;

  /// The output instance
  std::shared_ptr<hyro::ChannelOutput<Signal>> m_output;
};

} // namespace hyro

#endif // HYRO_SIGNALGENERATORCOMPONENT_H