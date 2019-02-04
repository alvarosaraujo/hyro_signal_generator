#include <iostream>

#include <hyro/DigitalConverterComponent.h> 

using namespace std::chrono_literals;

namespace hyro
{

// Where should I declare those variables to be accessible to all functions?

// Thresholding thold;
std::shared_ptr<HyroLogger> DigitalConverterComponent::s_logger = HyroLoggerManager::CreateLogger("DigitalConverterComponent");

Result
DigitalConverterComponent::init (const ComponentConfiguration & configuration)
{

  thold = Thresholding();
  auto m_dummy = registerOutput<std::vector<int>>("fix_dynamic"_uri, configuration);

  m_input = this->registerInput<Signal>("value"_uri, configuration);
  m_output = this->registerOutput<double>("digital"_uri, configuration);
  
  double amplitude = configuration.parameters.getParameter<double>("amplitude", 4);
  thold.setAmplitude(amplitude);

  double threshold = configuration.parameters.getParameter<double>("threshold", -1);
  thold.setThreshold(threshold);


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
  double val = value->value;

  //Calculates digital output from received value
  double res = thold.calculateThreshold(val);

  //Send result
  m_output->sendAsync(res);
}

} // namespace hyro

