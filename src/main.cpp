#include "FCFS.hpp"
#include "SJF.hpp"

int main() {
    std::vector<Process> processList = {
        {"P1", 5, 2}, // Arrival time: 5, Burst time: 2
        {"P2", 3, 5}, // Arrival time: 3, Burst time: 5
        {"P3", 2, 7}, // Arrival time: 2, Burst time: 7
    };

    // FCFS : P3, P2, P1
    // SJF : P1, P2, P3

    FCFS fcfs(processList);
    fcfs.run();
    // fcfs.exportToCSV();

    // SJF sjf(processList);
    // sjf.run();
    // sjf.exportToCSV();

    return 0;
}
