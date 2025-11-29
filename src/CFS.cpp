#include "CFS.hpp"

#include <iostream>
#include <thread>

void CFS::addArrivedProcesses() {
    for (Process& p : m_ProcessList) {
        if (m_TimePassed == p.arrivalTime && !p.finished) {
            std::cout << "[" << m_TimePassed << "] " << p.name << " has arrived\n";
            p.vruntime = getLowestVruntime();
            m_RunQueue[{p.vruntime, p.name}] = &p;
        }
    }
}

void CFS::executeProcess(Process* process) {
    std::cout << "[" << m_TimePassed << "] " << process->name << " is executing ("
              << process->remainingTime << "/" << process->burstTime << ") (" << process->vruntime
              << ")" << "\n";
    process->remainingTime--;
    m_TimePassed++;

    updateRuntime(process);

    // sleep
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));

    if (process->remainingTime <= 0) {
        finish(process);
        m_RunQueue.erase({process->vruntime, process->name});
    }
}

void CFS::updateRuntime(Process* process) {
    m_RunQueue.erase({process->vruntime, process->name});

    // process->vruntime++;
    process->vruntime += process->priority;

    m_RunQueue[{process->vruntime, process->name}] = process;
}

// Get the process with the lowest vruntime
Process* CFS::selectProcess() {
    if (m_RunQueue.empty())
        return nullptr;

    return m_RunQueue.begin()->second;
}

// Get lowest vruntime inside the map
long CFS::getLowestVruntime() {
    if (m_RunQueue.empty())
        return 0;

    return m_RunQueue.begin()->first.first;
}

// void CFS::printStats() {
//     std::cout << "Process Statistics:\n";
//     std::cout << std::string(70, '-') << "\n";
//     std::cout << "PID | Arrival | Burst | Completion | Turnaround | Waiting | Priority\n";
//     std::cout << std::string(70, '-') << "\n";
//
//     for (const Process& p : m_ProcessList) {
//         std::cout << std::setw(3) << p.name << " | " << std::setw(7) << p.arrivalTime << " | "
//                   << std::setw(5) << p.burstTime << " | " << std::setw(10) << p.completionTime
//                   << " | " << std::setw(10) << p.turnAroundTime << " | " << std::setw(7)
//                   << p.waitingTime << " | ";
//
//         switch (p.priority) {
//             case HIGH:
//                 std::cout << "HIGH\n";
//                 break;
//             case MEDIUM:
//                 std::cout << "MEDIUM\n";
//                 break;
//             case NORMAL:
//                 std::cout << "NORMAL\n";
//                 break;
//             case LOW:
//                 std::cout << "LOW\n";
//                 break;
//         }
//     }
// }
