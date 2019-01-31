#include <iostream>

#include <hyro/factory/CommandFactory.h>
#include <hyro/DigitalConverterComponent.h> 

using namespace std::chrono_literals;

namespace hyro
{

// Where should I declare those variables to be accessible to all functions?

// How to initialize parameters with config.parameters.getParameter?

// Use 'check' with both testing and main code

Thresholding thold;
std::shared_ptr<HyroLogger> DigitalConverterComponent::s_logger = HyroLoggerManager::CreateLogger("DigitalConverterComponent");

Result
DigitalConverterComponent::init (const ComponentConfiguration & configuration)
{
  auto m_dummy = registerOutput<std::vector<int>>("fix_dynamic"_uri, configuration);

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
  thold.setThreshold(th);
  return true;
}

// Get threshold dynamic property
double 
DigitalConverterComponent::getThreshold() 
{ 
  return thold.getThreshold();
}

// Set amplitude dynamic property
bool 
DigitalConverterComponent::setAmplitude(double amp) 
{
  thold.setAmplitude(amp);
  return true; 
}

// Get amplitude dynamic property
double 
DigitalConverterComponent::getAmplitude() 
{ 
  return thold.getAmplitude();
}

Result
DigitalConverterComponent::check ()
{ 
  if (m_input->connectionStatus() != ConnectionStatus::CONNECTION_UP)
    return Result::RESULT_ERROR;
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
  //long int timestamp = value->timestamp;
  //std::string frame = value->frame_id;
  double val = value->value;

  //Calculates digital output from received value
  double res = thold.calculateThreshold(val);

  //Send result
  m_output->sendAsync(res);
}

} // namespace hyro

