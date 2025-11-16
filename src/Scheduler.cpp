#include "Scheduler.hpp"

#include <fstream>
#include <iostream>
#include <thread>

// TODO:
// - Gantt Chart?

Scheduler::Scheduler(std::vector<Process>& processList) : m_ProcessList(processList) {}

// Main Loop
void Scheduler::run() {
    std::cout << "Running " << getName() << "\n";
    while (true) {
        addArrivedProcesses();

        if (allProcessFinished())
            break;

        if (m_ReadyQueue.empty()) {
            m_TimePassed++;
            std::cout << "[" << m_TimePassed << "] CPU idle \n";
            // sleep
            std::this_thread::sleep_for(std::chrono::milliseconds(600));
            continue;
        }

        // Process in ready queue
        Process* currentProcess = selectProcess(); // Get process
        if (currentProcess)
            executeProcess(currentProcess); // Put in CPU
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
    // Copy ready queue
    std::vector<Process*> temp;
    while (!m_ReadyQueue.empty()) {
        temp.push_back(m_ReadyQueue.front());
        m_ReadyQueue.pop();
    }

    // Sort by arrival time
    for (int i = 0; i < temp.size(); i++) {
        for (int j = i + 1; j < temp.size(); j++) {
            if (temp[i]->arrivalTime > temp[j]->arrivalTime) {
                std::swap(temp[i], temp[j]);
            }
        }
    }

    // Add to ready queue
    for (auto* p : temp)
        m_ReadyQueue.push(p);

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
        std::this_thread::sleep_for(std::chrono::milliseconds(600));

        // Manage arrived processes
        addArrivedProcesses();
    }

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

void Scheduler::exportToCSV() {
    double avgWaitingTime = 0;
    double avgTurnAroundTime = 0;
    for (const auto& p : m_ProcessList) {
        avgWaitingTime += p.waitingTime;
        avgTurnAroundTime += p.turnAroundTime;
    }
    avgWaitingTime /= m_ProcessList.size();
    avgTurnAroundTime /= m_ProcessList.size();

    // write the average times of the algorithm used
    std::string fileName = getName() + ".csv";
    std::ofstream file(fileName);
    file << "Algorithm,Average Waiting Time,Average Turnaround Time\n";
    file << getName() << "," << avgWaitingTime << "," << avgTurnAroundTime << "\n";
    file.close();

    // write the times for each process

    // std::ofstream file("processList.csv");
    // file << "Algorithm,Name,ArrivalTime,BurstTime,CompletionTime,TurnaroundTime,WaitingTime\n";
    // for (const auto& p : m_ProcessList) {
    //     file << getName() << "," << p.name << "," << p.arrivalTime << "," << p.burstTime << ","
    //          << p.completionTime << "," << p.turnAroundTime << "," << p.waitingTime << "\n";
    // }
    // file.close();

    std::cout << "Exported to " << fileName << "\n";
}
