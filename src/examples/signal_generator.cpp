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


void input_data()
{
  //-------------------------------------------------
  // LOCAL VARIABLES
  //-------------------------------------------------

  double signal_amp;
  double freq;
  double digital_amp;
  double thrhold;
  int sin_or_cos;

  // Access the dynamic properties from the SignalGeneratorComponent
  hyro::DynamicPropertyAccess dynamic_property_access_signal("/signal"_uri);
  hyro::DynamicPropertyAccess dynamic_property_access_digital("/digital"_uri);

  char quit;
  do
  {
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
    dynamic_property_access_signal.set<int>("cosine", sin_or_cos);
    dynamic_property_access_digital.set<double>("amplitude", digital_amp);
    dynamic_property_access_digital.set<double>("threshold", thrhold);

    std::cout << "--- Type 'q' to stop ---" << std::endl;
    std::cin >> quit;
  } while (quit != 'q');

  std::cout << "Close all widget windows to end program!" << std::endl;
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
      "parameters: {"
        "amplitude: 1, frequency: 1, sine: 0"
      "},"
      "outputs: {"
        "value:       { protocol: 'grpc', ip: '0.0.0.0', port: '50051'},"
        "timestamp:   { protocol: 'grpc', ip: '0.0.0.0', port: '50052'},"
        "frame_id:    { protocol: 'grpc', ip: '0.0.0.0', port: '50053'},"
        "fix_dynamic: { protocol: 'grpc', ip: '0.0.0.0', port: '50055'}}"
      "}");              
    ComponentConfiguration digital_conf = ComponentConfiguration("{"
      "parameters: {"
        "amplitude: 1, threshold: 1"
      "},"
      "outputs: {"
        "digital:     { protocol: 'grpc', ip: '0.0.0.0', port: '50056'},"
        "fix_dynamic: { protocol: 'grpc', ip: '0.0.0.0', port: '50054'}"
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
        "parameters: {"
          "amplitude: 5, frequency: 0.3, sine: 0"
        "},"
        "outputs: {"
          "value: { protocol: 'api' },"
          "timestamp: { protocol: 'api' },"
          "frame_id: { protocol: 'api' },"
          "fix_dynamic: { protocol: 'api' }"
        "}"
      "}");
    ComponentConfiguration digital_conf = ComponentConfiguration("{"
      "parameters: {"
        "amplitude: 5, threshold: 0"
      "},"
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

  //-------------------------------------------------
  // Step 9: Create spinners
  //-------------------------------------------------

  StateMachineSpinner signal_spinner(signal_sm, cancellation_token, 10ms);
  StateMachineSpinner digital_spinner(digital_sm, cancellation_token, 10ms);

  //-------------------------------------------------
  // Step 9: Create thread for input request
  //-------------------------------------------------

  std::this_thread::sleep_for(2s);
  std::thread thread_input(input_data);
  
  //-------------------------------------------------
  // Step 10: Start plot
  //-------------------------------------------------

  widgets::registerChannelListener<hyro::Signal>("/signal/value", "api", [](hyro::Signal sgn){
    widgets::plot2d<float>("Signal", "/signal/value", sgn.value);
  });

  widgets::registerChannelListener<double>("/digital/digital", "api", [](double dgt){
    widgets::plot2d<float>("Digital", "/digital/digital", dgt);
  });
  widgets::exec();
  
  std::this_thread::sleep_for(1s);

  //-------------------------------------------------
  // Step 11: Invokes cancellation token
  //-------------------------------------------------

  cancellation_token.cancel();
  
  //-------------------------------------------------
  // Step 12: Reset all state machines and exit
  //-------------------------------------------------


  
  signal_spinner.wait();
  digital_spinner.wait();
  thread_input.join();
  signal_sm.reset();
  digital_sm.reset();
  widgets::reset();

  return 0;
}

