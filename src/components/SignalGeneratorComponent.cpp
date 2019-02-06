#include <iostream>

#include <hyro/SignalGeneratorComponent.h>

using namespace std::chrono_literals;

namespace hyro
{
std::shared_ptr<HyroLogger> SignalGeneratorComponent::s_logger = HyroLoggerManager::CreateLogger("SignalGeneratorComponent");

Result
SignalGeneratorComponent::init (const ComponentConfiguration & configuration)
{
  std::shared_ptr<ChannelOutput<Signal>> m_signal_output;
  hyro::DynamicPropertyAccess dynamic_property_access_signal("/signal"_uri);

  double frequency = configuration.parameters.getParameter<double>("frequency", 1.0);
  m_signal_gen.setFrequency(frequency);

  double amplitude = configuration.parameters.getParameter<double>("amplitude", 1.0);
  m_signal_gen.setAmplitude(amplitude);

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

  registerDynamicProperty<int>("cosine",
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
  return m_signal_gen.getAmplitude();
}

bool 
SignalGeneratorComponent::setAmplitude(double ampl) 
{ 
  m_signal_gen.setAmplitude(ampl);
  return true;
}

double
SignalGeneratorComponent::getFrequency()
{
  return m_signal_gen.getFrequency();
}

bool 
SignalGeneratorComponent::setFrequency(double freq) 
{  
  m_signal_gen.setFrequency(freq);
  return true;
}

int
SignalGeneratorComponent::getCosine()
{
  return m_cosine;
}

bool 
SignalGeneratorComponent::setCosine(int coss) 
{ 
  m_signal_gen.setCosine(coss);
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
  
  //time_t t = std::time(0);
  //message_signal.timestamp = t;

  message_signal.timestamp = TimeUtils::Now_TimeSinceEpoch();
  message_signal.value = m_signal_gen.getSignalValue();
  message_signal.frame_id = "signal";

  m_output->sendAsync(message_signal);
  return Result::RESULT_OK;
}

} // namespace hyro

