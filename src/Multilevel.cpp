#include "Multilevel.hpp"
#include <iostream>
#include <thread>

void Multilevel::separateQueue() {
    // while (!m_ReadyQueue.empty()) {
    int queueSize = m_ReadyQueue.size();
    for (int i = 0; i < queueSize; i++) {
        Process* p = m_ReadyQueue.front();
        m_ReadyQueue.pop();
        switch (p->priority) {
            case PriorityLevel::HIGH:
                m_SystemQueue.push(p);
                break;
            case PriorityLevel::MEDIUM:
                m_InteractiveQueue.push(p);
                break;
            case PriorityLevel::LOW:
                m_BatchQueue.push(p);
                break;
        }
    }
}

Process* Multilevel::selectProcess() {
    separateQueue();

    // RR
    if (!m_SystemQueue.empty()) {
        Process* p = m_SystemQueue.front();
        m_SystemQueue.pop();
        std::cout << "Selected from System Queue: " << p->name << " (uses RR)\n";
        return p;
    }

    // FCFS
    if (!m_InteractiveQueue.empty()) {
        Process* p = m_InteractiveQueue.front();
        m_InteractiveQueue.pop();
        std::cout << "Selected from Interactive Queue: " << p->name << " (uses FCFS)\n";
        return p;
    }

    // SJF
    if (!m_BatchQueue.empty()) {
        std::vector<Process*> temp;
        while (!m_BatchQueue.empty()) {
            temp.push_back(m_BatchQueue.front());
            m_BatchQueue.pop();
        }

        // Sort by remaining time (shortest first)
        for (int i = 0; i < temp.size(); i++) {
            for (int j = i + 1; j < temp.size(); j++) {
                if (temp[i]->remainingTime > temp[j]->remainingTime) {
                    std::swap(temp[i], temp[j]);
                }
            }
        }

        Process* shortest = temp.front();

        for (auto* p : temp) {
            if (p != shortest)
                m_BatchQueue.push(p);
        }

        std::cout << "Selected from Batch Queue: " << shortest->name << " (uses SJF)\n";
        return shortest;
    }

    return nullptr;
}

void Multilevel::executeProcess(Process* process) {
    if (process->priority == PriorityLevel::HIGH) {
        roundRobinExecute(process);
    } else if (process->priority == PriorityLevel::MEDIUM) {
        FCFSExecute(process);
    } else if (process->priority == PriorityLevel::LOW) {
        SJFExecute(process);
    }
}

void Multilevel::roundRobinExecute(Process* process) {
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
    }

    // Process needs to be preempted
    if (process->remainingTime > 0) {
        std::cout << "[" << m_TimePassed << "] " << process->name << " has been preempted\n";
        m_SystemQueue.push(process);
    }

    // Process finished
    if (process->remainingTime <= 0)
        finish(process);
}

void Multilevel::FCFSExecute(Process* process) {
    if (process->remainingTime > 0) {
        m_TimePassed++;
        std::cout << "[" << m_TimePassed << "] Executing " << process->name << " (FCFS) ("
                  << process->remainingTime << "/" << process->burstTime << ")\n";
        process->remainingTime--;

        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
    }

    if (process->remainingTime <= 0) {
        finish(process);
        return;
    }

    m_InteractiveQueue.push(process);
}

void Multilevel::SJFExecute(Process* process) {
    if (process->remainingTime > 0) {
        m_TimePassed++;
        std::cout << "[" << m_TimePassed << "] Executing " << process->name << " (SJF) ("
                  << process->remainingTime << "/" << process->burstTime << ")\n";
        process->remainingTime--;

        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
    }

    if (process->remainingTime <= 0) {
        finish(process);
        return;
    }

    m_BatchQueue.push(process);
}
