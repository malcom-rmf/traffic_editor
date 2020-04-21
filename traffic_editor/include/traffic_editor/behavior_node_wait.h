/*
 * Copyright (C) 2019-2020 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifndef BEHAVIOR_NODE_WAIT_H
#define BEHAVIOR_NODE_WAIT_H

#include "behavior_node.h"
#include <yaml-cpp/yaml.h>

class BehaviorNodeWait : public BehaviorNode
{
public:
  double seconds = 0.0;
  double elapsed_seconds = 0.0;
  bool release_reservations = false;
  bool first_tick = false;

  BehaviorNodeWait(
      const double _seconds,
      const bool release_reservations = false);

  BehaviorNodeWait(const YAML::Node& y);
  ~BehaviorNodeWait();

  virtual void print() const;

  virtual void tick(
      const double dt_seconds,
      ModelState& state,
      Building& building,
      const std::vector<std::string>& inbound_messages,
      std::vector<std::string>& outbound_messages
  ) override;

  bool is_complete() const override;
};

#endif