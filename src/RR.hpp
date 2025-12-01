#pragma once

#include "Scheduler.hpp"
#include <iostream>
#include <thread>

enum class RRType { STATIC, DYNAMIC, DYNAMIC2 };

class RR : public Scheduler {
  public:
    RR(std::vector<Process> processList, RRType type = RRType::STATIC) :
        Scheduler(processList), m_RRType(type) {}

  private:
    void executeProcess(Process* process) override {
        // In CPU
        int usedTime = 0;
        while (usedTime < m_Quantum && process->remainingTime > 0) {
            if (m_RRType == RRType::DYNAMIC) {
                m_Quantum = getQuantum();
            } else if (m_RRType == RRType::DYNAMIC2) {
                m_Quantum = 20.0 / getTotalProcessInQueue();
            }
            std::cout << "[" << m_TimePassed << "] Quantum: " << m_Quantum << std::endl;

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

    // Get average burst time as quantum
    double getQuantum() {
        double totalBurstTime = 0;
        for (auto& p : m_ProcessList) {
            // Skip processes that have not arrived or are finished
            if (p.arrivalTime > m_TimePassed || p.finished)
                continue;
            totalBurstTime += p.burstTime;
        }
        m_Quantum = totalBurstTime / m_ProcessList.size();
        return m_Quantum;
    }

    int getTotalProcessInQueue() {
        int processInQueue = 0;
        for (auto& p : m_ProcessList) {
            // Skip processes that have not arrived or are finished
            if (p.arrivalTime > m_TimePassed || p.finished)
                continue;
            processInQueue++;
        }
        return processInQueue;
    }

    std::string getName() const override {
        if (m_RRType == RRType::STATIC)
            return "RR";
        else if (m_RRType == RRType::DYNAMIC)
            return "RRDynamic";
        else
            return "RRDynamic2";
    }

  private:
    double m_Quantum = 1;

    RRType m_RRType = RRType::STATIC;
};
