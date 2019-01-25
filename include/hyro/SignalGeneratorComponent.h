#pragma once

#ifndef HYRO_CORE_SIGNALGENERATORCOMPONENT_H
#define HYRO_CORE_SIGNALGENERATORCOMPONENT_H

#include <hyro_signal_generator_components_export.h>  

#include <string>
#include <hyro/msgs/Signal.h>
#include <hyro/core/Component.h>
#include <hyro/msgs/common/Basic.h>

namespace hyro
{

class HYRO_SIGNAL_GENERATOR_COMPONENTS_EXPORT SignalGeneratorComponent : public Component
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
  setCosine(bool cosine);
  
  bool 
  getCosine();

private:

  double m_amplitude;
  double m_frequency;
  bool m_cosine;
  
  double amplitude, frequency;
  bool cosine;

  static std::shared_ptr<HyroLogger> s_logger;

  /// The output instance
  std::shared_ptr<hyro::ChannelOutput<Signal>> m_output;
};

} // namespace hyro

#endif // HYRO_CORE_SIGNALGENERATORCOMPONENT_H