#include <hyro/SignalGenerator.h>
#include <hyro/SignalGeneratorComponent.h>
#include <hyro/Thresholding.h>
#include <hyro/DigitalConverterComponent.h>
#include <hyro/utils/DynamicPropertyAccess.h>

#include <hyro/test/ComponentTester.h>
#include <gtest/gtest.h>

static void
__assert_and_run_state_machine(hyro::StateMachine& sm,
                               const hyro::ComponentConfiguration& conf,
                               const hyro::ConnectionConfiguration& endpoint)
{
  ASSERT_EQ(hyro::Result::RESULT_OK, sm.init(conf));

  ASSERT_EQ(hyro::Result::RESULT_OK, sm.start());

  ASSERT_EQ(hyro::Result::RESULT_OK, sm.connect(endpoint));

  ASSERT_EQ(hyro::Result::RESULT_OK, sm.check());
}

#define ASSERT_AND_RUN_STATE_MACHINE(sm, conf, endpoint) \
  __assert_and_run_state_machine(sm, conf, endpoint)


namespace hyro
{
namespace test
{

//-------------------------------------------------
// Create test for Dynamic Properties
//-------------------------------------------------
TEST (ExampleTest, DynamicPropertiesCheck)
{
  //Configuration
  auto signal_conf = "{"
    "outputs: {"
      "value: { protocol: 'api' },"
      "timestamp: { protocol: 'api' },"
      "frame_id: { protocol: 'api' },"
      "fix_dynamic: { protocol: 'api' }"
      "}"
    "}";

  auto digital_conf = "{"
    "outputs: {"
      "digital: { protocol: 'api' },"
      "fix_dynamic: { protocol: 'api' }"
    "},"
    "inputs: {"
      "value: { protocol: 'api' }"
  "}}";

  //SignalGeneratorComponent and DigitalConverterComponent state machines
  StateMachine signal_generator_sm(std::make_shared<SignalGeneratorComponent>("/signal"_uri));
  StateMachine digital_converter_state_machine(std::make_shared<DigitalConverterComponent>("/digital"_uri));

  ASSERT_EQ(Result::RESULT_OK, signal_generator_sm.init(ComponentConfiguration(signal_conf)));
  ASSERT_EQ(Result::RESULT_OK, signal_generator_sm.start());
  ASSERT_EQ(Result::RESULT_OK, signal_generator_sm.connect(ConnectionConfiguration()));
  ASSERT_EQ(Result::RESULT_OK, signal_generator_sm.check());

  ASSERT_EQ(Result::RESULT_OK, digital_converter_state_machine.init(ComponentConfiguration(digital_conf)));
  ASSERT_EQ(Result::RESULT_OK, digital_converter_state_machine.start());
  ASSERT_EQ(Result::RESULT_OK, digital_converter_state_machine.connect(ConnectionConfiguration()));
  //ASSERT_EQ(Result::RESULT_OK, digital_converter_state_machine.check());

  // Access the dynamic properties from the SignalGeneratorComponent
  DynamicPropertyAccess dynamic_property_access_signal("/signal"_uri);
  DynamicPropertyAccess dynamic_property_access_digital("/digital"_uri);
  
  float sm_not_exits, dc_not_exits;;
  ASSERT_ANY_THROW(dynamic_property_access_digital.get("not_exists", sm_not_exits));
  ASSERT_ANY_THROW(dynamic_property_access_digital.get("not_exists", dc_not_exits));

  double signal_amplitude, digital_amplitude, frequency, threshold;
  int cosine;

  ASSERT_TRUE(dynamic_property_access_signal.get("amplitude", signal_amplitude));
  ASSERT_TRUE(dynamic_property_access_signal.get("frequency", frequency));
  ASSERT_TRUE(dynamic_property_access_signal.get("cosine", cosine));
  ASSERT_TRUE(dynamic_property_access_digital.get("threshold", threshold));

  std::cout << "PIMBA!" << std::endl;
  ASSERT_TRUE(dynamic_property_access_digital.get("amplitude", digital_amplitude));

  ASSERT_EQ(3, signal_amplitude);
  ASSERT_EQ(0.2, frequency);
  ASSERT_EQ(0, cosine);
  ASSERT_EQ(-1, threshold);
  ASSERT_EQ(4, digital_amplitude);
  
  signal_generator_sm.reset();
  digital_converter_state_machine.reset();
}

//-------------------------------------------------
// Create test for SignalGeneratorComponent
//-------------------------------------------------
TEST (ExampleTest, SignalGeneratorComponentCheck)
{
  //Configuration
  auto signal_conf = "{"
      "outputs: {"
        "value:       { protocol: 'api' },"
        "timestamp:   { protocol: 'api' },"
        "frame_id:    { protocol: 'api' },"
        "fix_dynamic: { protocol: 'api' }"
      "}"
    "}";

  //SignalGeneratorComponent state machine
  StateMachine signal_generator_sm(std::make_shared<SignalGeneratorComponent>("/signal"_uri));
 
  ASSERT_EQ(Result::RESULT_OK, signal_generator_sm.init(ComponentConfiguration(signal_conf)));
  ASSERT_EQ(Result::RESULT_OK, signal_generator_sm.start());
  ASSERT_EQ(Result::RESULT_OK, signal_generator_sm.connect(ConnectionConfiguration()));
  ASSERT_EQ(Result::RESULT_OK, signal_generator_sm.check());

  //Fake input channels
  auto inputSignal = std::make_shared<FakeInput<Signal>>("/signal"_uri, "api", "/signal/value");

  ASSERT_TRUE(inputSignal->connect());

  //TEST
  signal_generator_sm.update();

  auto valueSignal = std::shared_ptr<const Signal>();

  ReceiveStatus rec_signal = inputSignal->receive(valueSignal, 500ms);

  ASSERT_EQ(ReceiveStatus::RECEIVE_OK, rec_signal);
  EXPECT_EQ(valueSignal->value, 3);

  signal_generator_sm.reset();
}

//-------------------------------------------------
// Create test for DigitalConverterComponent
//-------------------------------------------------
TEST (ExampleTest, DigitalConverterComponentCheck)
{
  //-------------------------------------------------
  // Step 1: Create connection configuration string
  //-------------------------------------------------
  auto digital_conf = "{"
    "outputs: {"
      "digital: { protocol: 'api' },"
      "fix_dynamic: { protocol: 'api' }"
    "},"
    "inputs: {"
      "value: { protocol: 'api' }"
  "}}";

  std::string connect_configuration =
  "{"
     "value: {"
       "endpoint: '/digital/digital' "
     "}"
 "}";

  //-------------------------------------------------
  // Step 2: Create DigitalConverterComponent state
  //         machine
  //-------------------------------------------------

  //Fake output channels
  auto outputSignal = std::make_shared<FakeOutput<Signal>>("/digital"_uri, "api");
  ASSERT_TRUE(outputSignal->start());

  StateMachine digital_converter_state_machine(std::make_shared<DigitalConverterComponent>("/digital"_uri));

  ASSERT_AND_RUN_STATE_MACHINE(digital_converter_state_machine, ComponentConfiguration(digital_conf), ConnectionConfiguration(connect_configuration));

  //Fake input channels
  auto inputSignal = std::make_shared<FakeInput<Signal>>("/digital"_uri, "api", "/digital");
  ASSERT_TRUE(inputSignal->connect());

  //TEST
  digital_converter_state_machine.update();

  Signal message_sgn;
  message_sgn.value = 2.0;
  outputSignal->sendAsync(message_sgn);

  auto valueSignal = std::shared_ptr<const Signal>();
  
  ASSERT_EQ(ReceiveStatus::RECEIVE_OK, inputSignal->receive(valueSignal, 500ms));
  EXPECT_EQ(valueSignal->value, 2);

  digital_converter_state_machine.reset();
}

//-------------------------------------------------
// MAIN FUNCTION
//-------------------------------------------------
int main (int argc, char **argv)
{
  // Run all tests
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

}
}



