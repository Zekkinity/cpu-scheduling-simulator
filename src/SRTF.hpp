#pragma once

#include "Scheduler.hpp"
#include <iostream>
#include <thread>

// Shortest Remaining Time First
class SRTF : public Scheduler {
  public:
    SRTF(std::vector<Process> processList) : Scheduler(processList) {}
    ~SRTF() override = default;

  private:
    Process* selectProcess() override {
        if (m_ReadyQueue.empty())
            return nullptr;

        // Copy ready queue
        std::vector<Process*> temp;
        while (!m_ReadyQueue.empty()) {
            temp.push_back(m_ReadyQueue.front());
            m_ReadyQueue.pop();
        }

        // Sort by remaining time
        for (int i = 0; i < temp.size(); i++) {
            for (int j = i + 1; j < temp.size(); j++) {
                if (temp[i]->remainingTime > temp[j]->remainingTime) {
                    std::swap(temp[i], temp[j]);
                }
            }
        }

        // add all other processes
        for (auto* p : temp)
            m_ReadyQueue.push(p);

        Process* shortest = m_ReadyQueue.front();
        m_ReadyQueue.pop();
        return shortest;
    }

    void executeProcess(Process* process) override {
        if (process->remainingTime > 0) {
            // In CPU
            m_TimePassed++;
            std::cout << "[" << m_TimePassed << "] Executing " << process->name << " ("
                      << process->remainingTime << "/" << process->burstTime << ")\n";
            process->remainingTime--;

            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));

            // Manage arrived processes
            // addArrivedProcesses();
        }

        // Process finished
        if (process->remainingTime <= 0) {
            finish(process);
            return;
        }

        m_ReadyQueue.push(process);
    }

    std::string getName() const override { return "SRTF"; }
};
