#pragma once

#ifndef SIGNAL_GENERATOR_MSG_SIGNAL_PROTO_H
#define SIGNAL_GENERATOR_MSG_SIGNAL_PROTO_H

// This is important, otherwise you'll get compilation errors!
//#include <signal_generator_export.h>

#include <hyro/dynamic/ProtobufTraits.h>
#include <hyro/msgs/Signal.pb.h>

namespace hyro
{

template <>
struct ProtobufTraits<Signal> : public ProtobufTraitsDefault<Signal, msgs::Signal>
{
  static void
  FromMessage (const msgs::Signal & msg, Signal * value)
  {
    value->timestamp = hyro::Time{msg.timestamp()};
    value->value = msg.value();
    value->frame_id = msg.frame_id();
  }

  static void
  ToMessage (const Signal & value_in, msgs::Signal * msg)
  {
    msg->set_timestamp(value_in.timestamp.count());
    msg->set_frame_id(value_in.frame_id);
    msg->set_value(value_in.value);
  }

};

} // namespace hyro

#include <hyro/msgs/Signal.proto.h>

#endif // SIGNAL_GENERATOR_MSG_SIGNAL_PROTO_H
