#pragma once

#include "Scheduler.hpp"

#include <map>
#include <utility>
#include <vector>

// Completely Fair Scheduler
class CFS : public Scheduler {
  public:
    CFS(std::vector<Process> processList) : Scheduler(processList) {};
    ~CFS() = default;

  private:
    std::string getName() const override { return "CFS"; }

    Process* selectProcess() override;

    void addArrivedProcesses() override;
    void executeProcess(Process* process) override;

    void updateRuntime(Process* process);
    long getLowestVruntime();

    void printStats();

  private:
    using Key = std::pair<long, std::string>;
    std::map<Key, Process*> m_RunQueue; // {vruntime, pid} -> process*
};
