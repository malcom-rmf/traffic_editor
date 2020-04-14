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

#include <algorithm>
#include "behavior_node_await_signals.h"

using std::string;
using std::make_unique;


BehaviorNodeAwaitSignals::BehaviorNodeAwaitSignals(const YAML::Node& y)
: BehaviorNode()
{
  for (size_t i = 0; i < y[1].size(); i++)
    signal_names.push_back(y[1][i].as<string>());
}

BehaviorNodeAwaitSignals::~BehaviorNodeAwaitSignals()
{
}

void BehaviorNodeAwaitSignals::print() const
{
  printf("      await_signals:\n");
  for (const auto& signal_name : signal_names)
    printf("        %s\n", signal_name.c_str());
}

std::unique_ptr<BehaviorNode> BehaviorNodeAwaitSignals::instantiate(
      const YAML::Node& params,
      const std::string& _model_name) const
{
  std::unique_ptr<BehaviorNodeAwaitSignals> b =
      make_unique<BehaviorNodeAwaitSignals>(*this);
  b->model_name = _model_name;
  b->signal_names.clear();
  for (const auto& signal_name : signal_names)
  {
    b->signal_names.push_back(interpolate_string_params(signal_name, params));
    b->signals_received.push_back(false);
  }
  return b;
}

void BehaviorNodeAwaitSignals::tick(
    const double /*dt_seconds*/,
    ModelState& /*state*/,
    Building& /*building*/,
    const std::vector<std::unique_ptr<Model>>& /*active_models*/,
    const std::vector<std::string>& inbound_signals,
    std::vector<std::string>& /*outbound_signals*/)
{
  for (size_t i = 0; i < signal_names.size(); i++)
  {
    if (!signals_received[i])
    {
      if (std::find(inbound_signals.begin(),
              inbound_signals.end(),
              signal_names[i])
          != inbound_signals.end())
      {
        printf("RECEIVED: [%s]\n", signal_names[i].c_str());
        signals_received[i] = true;
        for (size_t j = 0; j < signals_received.size(); j++)
        {
          printf("  %zu: %d\n", j, signals_received[j] ? 1 : 0);
        }
      }
    }
  }
}

bool BehaviorNodeAwaitSignals::is_complete() const
{
  if (signal_names.empty())
    return true;
  for (auto signal_received : signals_received)
    if (!signal_received)
      return false;
  return true;
}