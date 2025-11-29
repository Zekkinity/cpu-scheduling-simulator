#pragma once

#include <string>

enum PriorityLevel { HIGH = 1, MEDIUM = 2, NORMAL = 4, LOW = 12 };

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
    long vruntime = 0; // Virtual runtime, lower value means higher priority

    Process(std::string name, float arrivalTime, float burstTime,
            PriorityLevel priority = PriorityLevel::MEDIUM) :
        name(name), arrivalTime(arrivalTime), burstTime(burstTime), priority(priority) {}
};
