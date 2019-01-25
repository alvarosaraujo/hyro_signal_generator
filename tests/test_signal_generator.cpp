#include <hyro/SignalGenerator.h>
#include <hyro/SignalGeneratorComponent.h>
#include <hyro/Thresholding.h>
#include <hyro/DigitalConverterComponent.h>
#include <hyro/utils/DynamicPropertyAccess.h>

#include <hyro/test/ComponentTester.h>
#include <gtest/gtest.h>

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
  StateMachine sm(std::make_shared<SignalGeneratorComponent>("/signal"_uri));
  StateMachine dc(std::make_shared<DigitalConverterComponent>("/digital"_uri));

  auto result_sm = sm.init(ComponentConfiguration(signal_conf));
  ASSERT_EQ(Result::RESULT_OK, result_sm);

  result_sm = sm.start();
  ASSERT_EQ(Result::RESULT_OK, result_sm);

  result_sm = sm.connect(ConnectionConfiguration());
  ASSERT_EQ(Result::RESULT_OK, result_sm);

  result_sm = sm.check();
  ASSERT_EQ(Result::RESULT_OK, result_sm);


  auto result_dc = dc.init(ComponentConfiguration(digital_conf));
  ASSERT_EQ(Result::RESULT_OK, result_dc);

  result_dc = dc.start();
  ASSERT_EQ(Result::RESULT_OK, result_dc);

  result_dc = dc.connect(ConnectionConfiguration());
  ASSERT_EQ(Result::RESULT_OK, result_dc);

  result_dc = dc.check();
  ASSERT_EQ(Result::RESULT_OK, result_dc);

  // Access the dynamic properties from the SignalGeneratorComponent
  DynamicPropertyAccess dynamic_property_access_signal("/signal"_uri);
  DynamicPropertyAccess dynamic_property_access_digital("/digital"_uri);
  
  float sm_not_exits, dc_not_exits;;
  ASSERT_ANY_THROW(dynamic_property_access_digital.get("not_exists", sm_not_exits));
  ASSERT_ANY_THROW(dynamic_property_access_digital.get("not_exists", dc_not_exits));
 
  double signal_amplitude, digital_amplitude, frequency, threshold;
  bool cosine;

  ASSERT_TRUE(dynamic_property_access_signal.get("amplitude", signal_amplitude));
  ASSERT_TRUE(dynamic_property_access_signal.get("frequency", frequency));
  ASSERT_TRUE(dynamic_property_access_signal.get("cosine", cosine));
  ASSERT_TRUE(dynamic_property_access_digital.get("threshold", threshold));
  ASSERT_TRUE(dynamic_property_access_digital.get("amplitude", digital_amplitude));

  ASSERT_EQ(0, signal_amplitude);
  ASSERT_EQ(0, frequency);
  ASSERT_EQ(0, cosine);
  ASSERT_EQ(0, threshold);
  ASSERT_EQ(0, digital_amplitude);
}

//-------------------------------------------------
// Create test for SignalGeneratorComponent
//-------------------------------------------------
TEST (ExampleTest, SignalGeneratorComponentCheck)
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

  //SignalGeneratorComponent state machine
  StateMachine sm(std::make_shared<SignalGeneratorComponent>("/signal"_uri));
 
  auto result = sm.init(ComponentConfiguration(signal_conf));
  ASSERT_EQ(Result::RESULT_OK, result);

  result = sm.start();
  ASSERT_EQ(Result::RESULT_OK, result);

  result = sm.connect(ConnectionConfiguration());
  ASSERT_EQ(Result::RESULT_OK, result);

  result = sm.check();
  ASSERT_EQ(Result::RESULT_OK, result);

  //Fake input channels
  auto inputSignal = std::make_shared<FakeInput<Signal>>("/signal"_uri, "api", "/signal/value");
  ASSERT_TRUE(inputSignal->connect());

  //TEST
  sm.update();

  auto valueSignal = std::shared_ptr<const Signal>();

  ReceiveStatus rec_signal = inputSignal->receive(valueSignal, 500ms);

  ASSERT_EQ(ReceiveStatus::RECEIVE_OK, rec_signal);
  EXPECT_EQ(valueSignal->value, 0);

  result = sm.reset();
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

  //-------------------------------------------------
  // Step 2: Create DigitalConverterComponent state
  //         machine
  //-------------------------------------------------
  StateMachine sm(std::make_shared<DigitalConverterComponent>("/digital"_uri));

  auto result = sm.init(ComponentConfiguration(digital_conf));
  ASSERT_EQ(Result::RESULT_OK, result);

  result = sm.start();
  ASSERT_EQ(Result::RESULT_OK, result);

  result = sm.connect(ConnectionConfiguration());
  ASSERT_EQ(Result::RESULT_OK, result);

  result = sm.check();
  ASSERT_EQ(Result::RESULT_OK, result);

  //Fake input channels
  auto inputSignal = std::make_shared<FakeInput<Signal>>("/digital"_uri, "api", "/digital/digital");
  ASSERT_TRUE(inputSignal->connect());

  //Fake output channels
  auto outputSignal = std::make_shared<FakeOutput<Signal>>("/digital"_uri, "api");
  ASSERT_TRUE(outputSignal->start());

  // hyro::Signal message_sgn;
  // message_sgn.value = 2.0;

  // outputSignal->sendAsync(message_sgn);

  // // //TEST
  // sm.update();

  // auto valueSignal = std::shared_ptr<const Signal>();
  
  // ReceiveStatus rec_signal = inputSignal->receive(valueSignal, 500ms);

  // ASSERT_EQ(ReceiveStatus::RECEIVE_OK, rec_signal);
  // EXPECT_EQ(valueSignal->value, 1);

  result = sm.reset();
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



