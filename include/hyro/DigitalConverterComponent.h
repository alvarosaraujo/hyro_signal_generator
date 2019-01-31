#pragma once

#ifndef HYRO_CORE_DIGITALCONVERTERCOMPONENT_H
#define HYRO_CORE_DIGITALCONVERTERCOMPONENT_H

#include <hyro_signal_generator_components_export.h>

#include <string>
#include <hyro/msgs/Signal.h>
#include <hyro/core/Component.h>
#include <hyro/utils/ISpinner.h>
#include <hyro/Thresholding.h>
#include <hyro/msgs/Signal.h>
#include <hyro/utils/SpinnerDefault.h>
#include <hyro/utils/SpinnerRated.h>
#include <hyro/msgs/common/Basic.h>

namespace hyro
{

class HYRO_SIGNAL_GENERATOR_COMPONENTS_EXPORT DigitalConverterComponent : public Component
{

public:

  using Component::Component;

  virtual
  ~DigitalConverterComponent () = default;

  // State transitions

  virtual Result
  reset () override;

  virtual Result
  init (const ComponentConfiguration & configuration) override;

  virtual Result
  start () override;

  virtual Result
  check () override;

  virtual Result
  update () override;

  void
  callback (std::shared_ptr<const Signal> && value);

  bool 
  setThreshold(double th);

  double 
  getThreshold();
  
  bool 
  setAmplitude(double amplitude);
  
  double 
  getAmplitude();

private:

  double m_amplitude, m_threshold;

  static std::shared_ptr<HyroLogger> s_logger;
  std::shared_ptr<hyro::ChannelInput<Signal>> m_input;
  std::shared_ptr<hyro::ChannelOutput<double>> m_output;
  std::unique_ptr<ISpinner> m_spinner;
};

} // namespace hyro

#endif // HYRO_CORE_SIGNALGENERATORCOMPONENT_H
