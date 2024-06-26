#pragma once

#include "Bot.h"

#include <memory>

namespace Engine_NS {
class Engine {
public:
  void receiveCommand(std::string_view command) const;

private:
  void handlePositionCommand(std::string_view command) const;

  std::unique_ptr<Bot> m_bot = std::make_unique<Bot>();
};
} // namespace Engine_NS