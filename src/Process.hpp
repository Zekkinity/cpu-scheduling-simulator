#pragma once

#include <string>

enum class PriorityLevel {
    LOW = 0,
    MEDIUM,
    HIGH,
};

struct Process {
    std::string name;
    int burstTime;   // Time needed in CPU
    int arrivalTime; // Time when

    int remainingTime = burstTime;
    int completionTime; // Time when the process is completed
    int turnAroundTime; // time spent in CPU
    int waitingTime;    // Time from arrival to completion

    bool finished = false;

    PriorityLevel priority = PriorityLevel::MEDIUM;

    Process(std::string name, float arrivalTime, float burstTime,
            PriorityLevel priority = PriorityLevel::MEDIUM) :
        name(name), arrivalTime(arrivalTime), burstTime(burstTime), priority(priority) {}
};
