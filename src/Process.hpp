#pragma once

#include <string>

struct Process {
    std::string name;
    int burstTime;   // Time needed in CPU
    int arrivalTime; // Time when

    int remainingTime = burstTime;
    int completionTime; // Time when the process is completed
    int turnAroundTime; // time spent in CPU
    int waitingTime;    // Time from arrival to completion

    bool finished = false;

    Process(std::string name, float arrivalTime, float burstTime) :
        name(name), arrivalTime(arrivalTime), burstTime(burstTime) {}
};

