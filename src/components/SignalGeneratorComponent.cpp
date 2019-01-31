#include <iostream>
#include <hyro/utils/SpinnerDefault.h>
#include <hyro/factory/CommandFactory.h>
#include <hyro/SignalGeneratorComponent.h>
#include <hyro/SignalGenerator.h>
#include <hyro/msgs/Signal.h>
#include <hyro/utils/DynamicPropertyAccess.h>

using namespace std::chrono_literals;

namespace hyro
{
std::shared_ptr<HyroLogger> SignalGeneratorComponent::s_logger = HyroLoggerManager::CreateLogger("SignalGeneratorComponent");
SignalGenerator sg;

Result
SignalGeneratorComponent::init (const ComponentConfiguration & configuration)
{
  std::shared_ptr<ChannelOutput<Signal>> m_signal_output;
  hyro::DynamicPropertyAccess dynamic_property_access_signal("/signal"_uri);

  registerDynamicProperty<double>("amplitude",
                                  &SignalGeneratorComponent::setAmplitude,
                                  &SignalGeneratorComponent::getAmplitude,
                                  this
                                  );

  registerDynamicProperty<double>("frequency",
                                  &SignalGeneratorComponent::setFrequency,
                                  &SignalGeneratorComponent::getFrequency,
                                  this
                                  );

  registerDynamicProperty<bool>("cosine",
                                &SignalGeneratorComponent::setCosine,
                                &SignalGeneratorComponent::getCosine,
                                this
                                );

  std::shared_ptr<ChannelOutput<std::vector<int>>>
  m_dummy = this->registerOutput<std::vector<int>>("fix_dynamic"_uri, configuration);
  m_output = this->registerOutput<Signal>("value"_uri, configuration);

  return Result::RESULT_OK;
}

double
SignalGeneratorComponent::getAmplitude()
{
  return sg.getAmplitude();
}

bool 
SignalGeneratorComponent::setAmplitude(double ampl) 
{ 
  amplitude = ampl; 
  sg.setAmplitude(amplitude);
  return true;
}

double
SignalGeneratorComponent::getFrequency()
{
  return sg.getFrequency();
}

bool 
SignalGeneratorComponent::setFrequency(double freq) 
{ 
  frequency = freq; 
  sg.setFrequency(frequency);
  return true;
}

bool
SignalGeneratorComponent::getCosine()
{
  return cosine;
}

bool 
SignalGeneratorComponent::setCosine(bool coss) 
{ 
  cosine = coss; 
  sg.setCosine(cosine);
  return true;
}

Result
SignalGeneratorComponent::check ()
{ 
  return Result::RESULT_OK;
}

Result
SignalGeneratorComponent::start ()
{
  double freq = 1;
  sg.setFrequency(freq);
  return Result::RESULT_OK;
}

Result
SignalGeneratorComponent::reset ()
{
  return Result::RESULT_OK;
}

Result
SignalGeneratorComponent::update ()
{
  hyro::Signal message_signal;
  time_t t = std::time(0);

  message_signal.timestamp = t;
  message_signal.value = sg.getSignalValue();
  message_signal.frame_id = "pimba";
  m_output->sendAsync(message_signal);
  return Result::RESULT_OK;
}

} // namespace hyro

