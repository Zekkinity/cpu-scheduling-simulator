#pragma once

#include "Process.hpp"

#include <queue>
#include <vector>

class Scheduler {
  public:
    Scheduler(std::vector<Process>& processList);
    virtual ~Scheduler() = default;

    virtual void run();
    void exportToCSV();

  private:
    void addArrivedProcesses();
    bool allProcessFinished();

  protected:
    virtual Process* selectProcess();
    virtual void executeProcess(Process* process);

    virtual std::string getName() const = 0;

  protected:
    std::queue<Process*> m_ReadyQueue;
    int m_TimePassed = 0;
    bool m_AllDone = true;

    std::vector<Process> m_ProcessList;
};
