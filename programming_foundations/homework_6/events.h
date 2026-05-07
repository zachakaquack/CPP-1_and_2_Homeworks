#pragma once

#include "base_event.h"
#include <iostream>

class ConsoleLoop : public BaseEvent {
  public:
    using BaseEvent::BaseEvent;

    void mainLoop() override;
};

class FileLoop : public BaseEvent {
  public:
    using BaseEvent::BaseEvent;

    void mainLoop() override;
};
