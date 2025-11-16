#pragma once

#include "Scheduler.hpp"

// Shortest Job First
class SJF : public Scheduler {
  public:
    SJF(std::vector<Process>& processList) : Scheduler(processList) {}

  private:
    Process* selectProcess() override {
        // Copy ready queue
        std::vector<Process*> temp;
        while (!m_ReadyQueue.empty()) {
            temp.push_back(m_ReadyQueue.front());
            m_ReadyQueue.pop();
        }

        // TODO:
        // sort by remaining time?

        // sort by burst time
        for (int i = 0; i < temp.size(); i++) {
            for (int j = i + 1; j < temp.size(); j++) {
                if (temp[i]->burstTime > temp[j]->burstTime) {
                    std::swap(temp[i], temp[j]);
                }
            }
        }

        Process* shortest = temp.front();

        // add all other processes
        for (auto* p : temp)
            if (p != shortest)
                m_ReadyQueue.push(p);

        return shortest;
    }

    std::string getName() const override { return "SJF"; }
};
