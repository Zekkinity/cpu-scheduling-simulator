#pragma once

#include "Scheduler.hpp"
#include <iostream>
#include <thread>

class RR : public Scheduler {
  public:
    RR(std::vector<Process> processList) : Scheduler(processList) {}

  private:
    void executeProcess(Process* process) override {
        // In CPU
        int usedTime = 0;
        while (usedTime < m_Quantum && process->remainingTime > 0) {
            m_TimePassed++;
            usedTime++;
            std::cout << "[" << m_TimePassed << "] Executing " << process->name << " ("
                      << process->remainingTime << "/" << process->burstTime << ")\n";
            process->remainingTime--;

            // sleep
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));

            // Manage arrived processes
            addArrivedProcesses();
        }

        // Process needs to be preempted
        if (process->remainingTime > 0) {
            std::cout << "[" << m_TimePassed << "] " << process->name << " has been preempted\n";
            m_ReadyQueue.push(process);
        }

        // Process finished
        if (process->remainingTime <= 0)
            finish(process);
    }

    std::string getName() const override { return "RR"; }

  private:
    double m_Quantum = 1;
};
