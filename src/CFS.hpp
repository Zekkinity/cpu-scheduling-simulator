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

    void addArrivedProcesses() override;
    Process* selectProcess() override;
    void executeProcess(Process* process) override;

    void updateVruntime(Process* p);

    int getWeight(int nice);
    int getTotalWeight();
    double getMinVruntime() const;
    int computeSlice(Process* p);

  private:
    // NICE is a value of how nice a process is to others, so a low nice value equals to high
    // priority

    // The nice value is designed to be like:
    // down by 1 = 10% more CPU
    // up by 1 = 10% less CPU

    // RANGE [-20, 19]
    const int MIN_NICE = -20;
    const int MAX_NICE = 19;

    // Default nice of a process is 0
    const int DEFAULT_NICE = 0;

    // const int SCHED_LATENCY = 48; // too high

    const int SCHED_LATENCY = 32; // This is even better for Waiting Times
    // const int SCHED_LATENCY = 24; // This one provides results better than Dynamic RR

    // const int SCHED_LATENCY = 12; // Too low, GOOD for fairness
    const int MIN_GRANULARITY = 6;

    using Key = std::pair<double, std::string>;
    std::map<Key, Process*> m_RunQueue; // {vruntime, pid} -> process*
};
