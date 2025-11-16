#pragma once

#include "Scheduler.hpp"

class FCFS : public Scheduler {
  public:
    FCFS(std::vector<Process>& processList) : Scheduler(processList) {}

  private:
    Process* selectProcess() override {
        Process* currentProcess = m_ReadyQueue.front();
        m_ReadyQueue.pop();
        return currentProcess;
    }

    std::string getName() const override { return "FCFS"; }
};
