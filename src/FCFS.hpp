#pragma once

#include "Scheduler.hpp"

class FCFS : public Scheduler {
  public:
    FCFS(std::vector<Process> processList) : Scheduler(processList) {}

  private:
    std::string getName() const override { return "FCFS"; }
};
