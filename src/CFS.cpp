#include "CFS.hpp"

#include <cmath>
#include <iostream>
#include <thread>

void CFS::addArrivedProcesses() {
    for (Process& p : m_ProcessList) {
        if (m_TimePassed == p.arrivalTime && !p.finished) {
            std::cout << "[" << m_TimePassed << "] " << p.name << " has arrived\n";
            p.vruntime = getMinVruntime();
            m_RunQueue[{p.vruntime, p.name}] = &p;
        }
    }
}

// Get the process with the lowest vruntime
Process* CFS::selectProcess() {
    if (m_RunQueue.empty())
        return nullptr;

    return m_RunQueue.begin()->second;
}

void CFS::executeProcess(Process* process) {
    int slice = computeSlice(process);
    std::cout << "Slice: " << slice << "\n";

    for (int i = 0; i < slice; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));

        addArrivedProcesses();

        if (process->remainingTime == 0)
            break;

        process->remainingTime--;
        m_TimePassed++;
        updateVruntime(process);

        // std::cout << "[" << m_Time << "] PID " << p->pid << " remaining time: (" <<
        // p->remainingTime
        //           << "/" << p->burstTime << ") vr=" << p->vruntime << "\n";
        std::cout << "[" << m_TimePassed << "] " << process->name << " is executing ("
                  << process->remainingTime << "/" << process->burstTime << ") ("
                  << process->vruntime << ")" << "\n";

        // No longer has the minimum vruntime
        if (process->vruntime > getMinVruntime() && i >= MIN_GRANULARITY)
            break;
    }

    if (process->remainingTime <= 0) {
        finish(process);
        m_RunQueue.erase({process->vruntime, process->name});
    }
}

int CFS::computeSlice(Process* p) {
    double processWeight = getWeight(p->nice);

    // Proportion of CPU allocated
    double cpuShare = processWeight / getTotalWeight();
    std::cout << "PID " << p->name << " CPU allocated: " << 100 * cpuShare << "% \n";

    int slice = SCHED_LATENCY * cpuShare;
    slice = std::max(MIN_GRANULARITY, slice);

    return slice;
}

void CFS::updateVruntime(Process* p) {
    m_RunQueue.erase({p->vruntime, p->name});

    // vruntime += delta_exec * (1024 / weight)
    double processWeight = getWeight(p->nice);
    p->vruntime += 1 * (1024.0 / processWeight);
    std::cout << "PID " << p->name << " updated vruntime: " << p->vruntime << "\n";

    m_RunQueue.insert({{p->vruntime, p->name}, p});
}

int CFS::getWeight(int nice) {
    int weight = 1024 * std::pow(1.25, -nice);
    return weight;
}

// Sum of weights of all process
int CFS::getTotalWeight() {
    int totalWeight = 0;
    for (auto& [_, p] : m_RunQueue) {
        totalWeight += getWeight(p->nice);
    }
    return totalWeight;
}

double CFS::getMinVruntime() const {
    if (m_RunQueue.empty())
        return 0;

    return m_RunQueue.begin()->first.first;
}
