#pragma once

#include "Scheduler.hpp"
#include <iostream>
#include <thread>

class Priority : public Scheduler {
  public:
    Priority(std::vector<Process> processList) : Scheduler(processList) {}

    Process* selectProcess() override {
        if (m_ReadyQueue.empty())
            return nullptr;

        std::vector<Process*> temp;
        while (!m_ReadyQueue.empty()) {
            temp.push_back(m_ReadyQueue.front());
            m_ReadyQueue.pop();
        }

        // Sort by highest priority
        for (int i = 0; i < temp.size(); i++) {
            for (int j = i + 1; j < temp.size(); j++) {
                if (temp[i]->priority < temp[j]->priority) {
                    std::swap(temp[i], temp[j]);
                }
            }
        }

        for (auto* p : temp)
            m_ReadyQueue.push(p);

        Process* highestPriority = m_ReadyQueue.front();
        m_ReadyQueue.pop();

        std::cout << "Selected process: " << highestPriority->name << '\n';
        return highestPriority;
    }

    void executeProcess(Process* process) override {
        if (process->remainingTime > 0) {
            m_TimePassed++;
            std::cout << "[" << m_TimePassed << "] Executing " << process->name << " ("
                      << process->remainingTime << "/" << process->burstTime << ")\n";
            process->remainingTime--;

            // sleep
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
        }

        // Process finished
        if (process->remainingTime <= 0) {
            finish(process);
            return;
        }

        m_ReadyQueue.push(process);
    }

    std::string getName() const override { return "Priority"; }
};
