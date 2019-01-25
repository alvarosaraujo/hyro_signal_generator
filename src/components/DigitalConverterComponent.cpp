#include <iostream>

#include <hyro/factory/CommandFactory.h>
#include <hyro/DigitalConverterComponent.h> 

using namespace std::chrono_literals;

namespace hyro
{
hyro::Signal message_signal_rec;
hyro::Signal message_output;
Thresholding thold;
std::shared_ptr<HyroLogger> DigitalConverterComponent::s_logger = HyroLoggerManager::CreateLogger("DigitalConverterComponent");

Result
DigitalConverterComponent::init (const ComponentConfiguration & configuration)
{
  /**
   * @brief Initializes DigitalConverterComponent
   * 
   */

  std::shared_ptr<ChannelOutput<std::vector<int>>>
  m_dummy = this->registerOutput<std::vector<int>>("fix_dynamic"_uri, configuration);

  m_input = this->registerInput<Signal>("value"_uri, configuration);
  m_output = this->registerOutput<double>("digital"_uri, configuration);

  registerDynamicProperty<double>("amplitude", 
                                  &DigitalConverterComponent::setAmplitude,
                                  &DigitalConverterComponent::getAmplitude,
                                  this
                                  );

  registerDynamicProperty<double>("threshold", 
                                  &DigitalConverterComponent::setThreshold,
                                  &DigitalConverterComponent::getThreshold,
                                  this
                                  );

  // Always return a result code.
  return Result::RESULT_OK;
}

// Set threshold dynamic property
bool 
DigitalConverterComponent::setThreshold(double th) 
{ 
  threshold = th; 
  thold.setThreshold(threshold);
  return true;
}

// Get threshold dynamic property
double 
DigitalConverterComponent::getThreshold() 
{ 
  return threshold; 
}

// Set amplitude dynamic property
bool 
DigitalConverterComponent::setAmplitude(double amp) 
{
  amplitude = amp;
  thold.setAmplitude(amplitude);
  return true; 
}

// Get amplitude dynamic property
double 
DigitalConverterComponent::getAmplitude() 
{ 
  return amplitude; 
}

Result
DigitalConverterComponent::check ()
{ 
  return Result::RESULT_OK;
}

Result
DigitalConverterComponent::start ()
{
  //Create spinner with callback
  m_spinner = SpinnerDefault::Create(m_input, &DigitalConverterComponent::callback, this, this->suspensionToken());
  return Result::RESULT_OK;
}

Result
DigitalConverterComponent::reset ()
{
  return Result::RESULT_OK;
}

Result
DigitalConverterComponent::update ()
{
  return Result::RESULT_OK;
}

void
DigitalConverterComponent::callback (std::shared_ptr<const Signal> && value)
{
  //Retrieves data from received message
  std::string timestamp = value->timestamp;
  std::string frame = value->frame_id;
  double val = value->value;

  //Calculates digital output from received value
  double res = thold.get_threshold(val);

  //Send result
  m_output->sendAsync(res);
}

} // namespace hyro

