#pragma once

#ifndef SIGNAL_GENERATOR_MSG_SIGNAL_H
#define SIGNAL_GENERATOR_MSG_SIGNAL_H

#include <hyro_signal_generator_components_export.h>
//pimba

namespace hyro
{

struct HYRO_SIGNAL_GENERATOR_COMPONENTS_EXPORT Signal
{
  long int timestamp;
  std::string frame_id;
  double value;
};

} // namespace hyro

#include <hyro/msgs/Signal.proto.h>

#endif // SIGNAL_GENERATOR_MSG_SIGNAL_H
