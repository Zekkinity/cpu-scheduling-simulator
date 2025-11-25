#include "Scheduler.hpp"

#include <iostream>
#include <thread>

// TODO:
// - Gantt Chart?

// - FCFS (First-Come, First-Served)
// - SJF (Shortest Job First)
// - SRTF (Shortest Remaining Time First)
// - Round-Robin (RR)
// - Priority Scheduling
// - Multilevel Feedback Queue (MLFQ)

Scheduler::Scheduler(std::vector<Process> processList) : m_ProcessList(processList) {}

// Main Loop
void Scheduler::run() {
    std::cout << "Running " << getName() << "\n";
    while (true) {
        addArrivedProcesses();

        if (allProcessFinished())
            break;

        Process* currentProcess = selectProcess(); // Get process
        if (currentProcess) {
            executeProcess(currentProcess); // Put in CPU
        } else {
            m_TimePassed++;
            std::cout << "[" << m_TimePassed << "] CPU idle \n";
            // sleep
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
        }
    }
}

void Scheduler::addArrivedProcesses() {
    // Move Process into ready queue (real time)
    for (Process& p : m_ProcessList) {
        if (m_TimePassed == p.arrivalTime && !p.finished) {
            std::cout << "[" << m_TimePassed << "] " << p.name << " has arrived\n";
            m_ReadyQueue.push(&p);
        }
    }
}

Process* Scheduler::selectProcess() {
    if (m_ReadyQueue.empty())
        return nullptr;

    Process* currentProcess = m_ReadyQueue.front();
    m_ReadyQueue.pop();

    std::cout << "Selected process: " << currentProcess->name << '\n';
    return currentProcess;
}

void Scheduler::executeProcess(Process* process) {
    // In CPU
    while (process->remainingTime > 0) {
        m_TimePassed++;
        std::cout << "[" << m_TimePassed << "] Executing " << process->name << " ("
                  << process->remainingTime << "/" << process->burstTime << ")\n";
        process->remainingTime--;

        // sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));

        // Manage arrived processes
        addArrivedProcesses();
    }

    // Process finished
    if (process->remainingTime <= 0)
        finish(process);
}

void Scheduler::finish(Process* process) {
    // Complete process
    process->finished = true;
    process->completionTime = m_TimePassed;
    process->turnAroundTime = process->completionTime - process->arrivalTime;
    process->waitingTime = process->turnAroundTime - process->burstTime;

    std::cout << "[" << m_TimePassed << "] " << process->name << " has finished\n";
}

bool Scheduler::allProcessFinished() {
    for (Process& p : m_ProcessList) {
        if (!p.finished)
            return false;
    }
    return true;
}

void Scheduler::exportToCSV(std::string& file) {
    // write the times for each process
    for (const auto& p : m_ProcessList) {
        file += getName() + "," + p.name + "," + std::to_string(p.arrivalTime) + "," +
                std::to_string(p.burstTime) + "," + std::to_string(p.completionTime) + "," +
                std::to_string(p.waitingTime) + "," + std::to_string(p.turnAroundTime) + "\n";
    }
}
