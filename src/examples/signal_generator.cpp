//#define TEST_GRPC
//#define TEST_LCM
#define TEST_API

#include <bits/stdc++.h>
#include <thread>

#include <hyro/SignalGenerator.h>
#include <hyro/SignalGeneratorComponent.h>
#include <hyro/Thresholding.h>
#include <hyro/DigitalConverterComponent.h>

// We will work with state machines that wraps the components
#include <hyro/core/StateMachine.h>
#include <hyro/utils/components/PrintComponent.h>

// A utility that will manage multi-threaded updates for us (less code to write)
#include <hyro/utils/StateMachineSpinner.h>
#include <hyro/utils/components/NumberGeneratorComponent.h>
#include <hyro/utils/DynamicPropertyAccess.h>
#include <hyro/msgs/common/Basic.h>
#include <hyro/widgets/WidgetCollection.h>

using namespace hyro;
using namespace std::string_literals;
using namespace std::chrono_literals;

double signal_property_amplitude;
double signal_property_frequency;
bool signal_property_cosine;
double digital_property_amplitude;
double digital_property_threshold;

double signal_amp;
double freq;
double digital_amp;
double thrhold;
bool sin_or_cos;

void plot()
{
  widgets::registerChannelListener<hyro::Signal>("/signal/value", "api", [](hyro::Signal sgn){
    widgets::plot2d<float>("Signal", "/signal/value", sgn.value);
  });

  widgets::registerChannelListener<double>("/digital/digital", "api", [](double dgt){
    widgets::plot2d<float>("Digital", "/digital/digital", dgt);
  });
  widgets::exec();
}

int main()
{

  //-------------------------------------------------
  // MAIN FUNCTION
  //-------------------------------------------------

  // Disables loggin information
  hyro::LogConfig config;
  config.level = hyro::LogLevel::ERROR;
  hyro::HyroLoggerManager::Configure(config);

  //-------------------------------------------------
  // Step 1: Create state machines
  //-------------------------------------------------

  // State machines creation
  StateMachine signal_sm(std::make_shared<SignalGeneratorComponent>("/signal"_uri));
  StateMachine digital_sm(std::make_shared<DigitalConverterComponent>("/digital"_uri));


  //-------------------------------------------------
  // Step 2: Create component configuration strings
  //         for different communication protocols
  //-------------------------------------------------

  // GRPC
  #if defined(TEST_GRPC)
    ComponentConfiguration signal_conf = ComponentConfiguration("{"
      "outputs: {"
        "value:     {protocol: 'grpc', ip: '0.0.0.0', port: '50051'},"
        "timestamp: {protocol: 'grpc', ip: '0.0.0.0', port: '50052'},"
        "frame_id:  {protocol: 'grpc', ip: '0.0.0.0', port: '50053'}}"
      "}");              
    ComponentConfiguration digital_conf = ComponentConfiguration("{"
      "outputs: {"
        "digital: { protocol: 'grpc', ip: '0.0.0.0', port: '50054' }"
      "},"
      "inputs: {"
        "value:     { protocol: 'grpc' },"
        "timestamp: { protocol: 'grpc' },"
        "frame_id:  { protocol: 'grpc' }}"
      "}");

    auto remote_endpoint = ConnectionConfiguration("{"
      "value:     { ip: '0.0.0.0', port: '50051' },"
      "timestamp: { ip: '0.0.0.0', port: '50052' },"
      "frame_id: { ip: '0.0.0.0', port: '50053' }"
    "}");

  // LCM
  #elif defined(TEST_LCM)
      ComponentConfiguration signal_conf = ComponentConfiguration("{ outputs: { value: {protocol: 'lcm' } } }");
      ComponentConfiguration digital_conf = ComponentConfiguration("{ outputs: { digital: {protocol: 'lcm' } }, inputs: { value: {protocol: 'lcm'} } }");

      auto remote_endpoint = ConnectionConfiguration("{ number: { topic: '/signal/value' } }");

  // API
  #elif defined(TEST_API)
    ComponentConfiguration signal_conf = ComponentConfiguration("{"
        "outputs: {"
          "value: { protocol: 'api' },"
          "timestamp: { protocol: 'api' },"
          "frame_id: { protocol: 'api' },"
          "fix_dynamic: { protocol: 'api' }"
        "}"
      "}");
    ComponentConfiguration digital_conf = ComponentConfiguration("{"
      "outputs: {"
        "digital: { protocol: 'api' },"
        "fix_dynamic: { protocol: 'api' }"
      "},"
      "inputs: {"
        "value: { protocol: 'api' }"
    "}}");

    auto remote_endpoint = ConnectionConfiguration("{ value: { endpoint: '/signal/value' } }");

  #endif

  //-------------------------------------------------
  // Step 3: Initialize state machines
  //-------------------------------------------------

  signal_sm.init(signal_conf);
  digital_sm.init(digital_conf);

  //-------------------------------------------------
  // Step 4: Start state machines
  //-------------------------------------------------

  signal_sm.start();
  digital_sm.start();

  //-------------------------------------------------
  // Step 5: Connect state machines
  //-------------------------------------------------

  signal_sm.connect();
  digital_sm.connect(remote_endpoint);

  //-------------------------------------------------
  // Step 6: Check state machines
  //-------------------------------------------------

  signal_sm.check();
  digital_sm.check();

  //-------------------------------------------------
  // Step 7: Update state machines
  //-------------------------------------------------

  signal_sm.update();
  digital_sm.update();

  //-------------------------------------------------
  // Step 8: Create cancellation token and sleep for 
  //         1s
  //-------------------------------------------------

  // This object is a single token, shared by all the threads, used to ask them to stop running.
  CancellationTokenSource cancellation_token;
  std::this_thread::sleep_for(1s);

  //-------------------------------------------------
  // Step 9: Create spinners
  //-------------------------------------------------

  StateMachineSpinner signal_spinner(signal_sm, cancellation_token, 10ms);
  StateMachineSpinner digital_spinner(digital_sm, cancellation_token, 10ms);

  //-------------------------------------------------
  // Step 10: Create dynamic property access
  //-------------------------------------------------

  // Access the dynamic properties from the SignalGeneratorComponent
  hyro::DynamicPropertyAccess dynamic_property_access_signal("/signal"_uri);
  hyro::DynamicPropertyAccess dynamic_property_access_digital("/digital"_uri);

  //-------------------------------------------------
  // Step 11: Start plot function as thread
  //-------------------------------------------------

  std::thread t1(plot);
  std::this_thread::sleep_for(2s);

  //-------------------------------------------------
  // Step 12: Request parameters from user and set
  //          new dynamic properties
  //-------------------------------------------------

  while (true)
  {
    std::cout << "Type 'q' to exit" << std::endl;
    std::cout << "Type the sine amplitude: ";
    std::cin >> signal_amp;
    std::cout << "Type the sine frequency: ";
    std::cin >> freq;
    std::cout << "Type 0 for sine or 1 for cosine: ";
    std::cin >> sin_or_cos;
    std::cout << "Type the digital signal amplitude: ";
    std::cin >> digital_amp;
    std::cout << "Type the threshold: ";
    std::cin >> thrhold;

    // Set new values to dynamic properties
    dynamic_property_access_signal.set<double>("amplitude", signal_amp);
    dynamic_property_access_signal.set<double>("frequency", freq);
    dynamic_property_access_signal.set<bool>("cosine", sin_or_cos);
    dynamic_property_access_digital.set<double>("amplitude", digital_amp);
    dynamic_property_access_digital.set<double>("threshold", thrhold);
  }
  
  //-------------------------------------------------
  // Step 13: Invokes cancellation token
  //-------------------------------------------------
  cancellation_token.cancel();

  //-------------------------------------------------
  // Step 14: Reset all state machines and exit
  //-------------------------------------------------

  signal_sm.reset();
  digital_sm.reset();
  widgets::reset();
  return 0;
}

