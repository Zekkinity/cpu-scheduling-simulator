#pragma once

#include "Scheduler.hpp"

class Multilevel : public Scheduler {
  public:
    Multilevel(std::vector<Process> processList) : Scheduler(processList) {}
    ~Multilevel() override = default;

  private:
    Process* selectProcess() override;
    void executeProcess(Process* process) override;

    std::string getName() const override { return "MLQ"; }

    void separateQueue();
    void roundRobinExecute(Process* process);
    void FCFSExecute(Process* process);
    void SJFExecute(Process* process);

  private:
    std::queue<Process*> m_SystemQueue;      // High priority
    std::queue<Process*> m_InteractiveQueue; // Medium priority
    std::queue<Process*> m_BatchQueue;       // Low Priority

    int m_Quantum = 2;
};
