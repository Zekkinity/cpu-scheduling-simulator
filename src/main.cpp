#include "CFS.hpp"
#include "FCFS.hpp"
#include "Multilevel.hpp"
#include "Priority.hpp"
#include "RR.hpp"
#include "SJF.hpp"
#include "SRTF.hpp"

#include <cstdlib>
#include <ctime>
#include <fstream>

const int PROCESS_AMOUNT = 10;
const bool EXPORT_TO_CSV = true;
const std::string FILE_NAME = "scheduler_data.csv";

void generateRandomProcess(std::vector<Process>& processList) {
    for (int i = 0; i < PROCESS_AMOUNT; i++) {
        std::string name = "P" + std::to_string(i + 1);
        int arrivalTime = rand() % 20 + 1;
        int burstTime = rand() % 20 + 1;
        // std::cout << name << " " << arrivalTime << " " << burstTime << std::endl;
        processList.emplace_back(name, arrivalTime, burstTime);
    }
}

int handleMenu() {
    std::cout << "Select Algorithm: \n";
    std::cout << "1. FCFS (First Come First Served)\n";
    std::cout << "2. SJF (Shortest Job First)\n";
    std::cout << "3. SRTF (Shortest Remaining Time First)\n";
    std::cout << "4. RR (Round Robin)\n";
    std::cout << "5. RR (Dynamic)\n";
    std::cout << "6. RR (Dynamic 2)\n";
    std::cout << "7. Priority\n";
    std::cout << "8. MLQ (Multilevel Queue)\n";
    std::cout << "9. CFS (Completely Fair Scheduler)\n";
    std::cout << "10. Run All\n";
    std::cout << "0. Exit\n";
    std::cout << "Select: ";

    int selected;
    std::cin >> selected;
    return selected;
}

void runScheduler(Scheduler& scheduler, std::string& file) {
    scheduler.run();

    if (EXPORT_TO_CSV) {
        scheduler.exportToCSV(file);
    }
}

void runAllSchedulers(std::vector<Process> processList, std::string& file) {
    FCFS fcfs(processList);
    SJF sjf(processList);
    SRTF srtf(processList);
    RR rr(processList);
    RR rrd(processList, RRType::DYNAMIC);
    RR rrd2(processList, RRType::DYNAMIC2);
    Priority priority(processList);
    Multilevel mlfq(processList);
    CFS cfs(processList);

    runScheduler(fcfs, file);
    runScheduler(sjf, file);
    runScheduler(srtf, file);
    runScheduler(rr, file);
    runScheduler(rrd, file);
    runScheduler(rrd2, file);
    runScheduler(priority, file);
    runScheduler(mlfq, file);
    runScheduler(cfs, file);
}

int main() {
    srand(time(0));

    // std::vector<Process> processList;
    // generateRandomProcess(processList);

    // std::vector<Process> processList = {
    //     {"P1", 5, 2, PriorityLevel::HIGH},   // Arrival time: 5, Burst time: 2
    //     {"P2", 3, 5, PriorityLevel::MEDIUM}, // Arrival time: 3, Burst time: 5
    //     {"P3", 2, 7, PriorityLevel::LOW},    // Arrival time: 2, Burst time: 7
    //     {"P4", 6, 3, PriorityLevel::MEDIUM}, // Arrival time: 6, Burst time: 3
    // };

    // short_workload
    // std::vector<Process> processList = {
    //     {"P1", 1, 2, PriorityLevel::MEDIUM}, {"P2", 2, 1, PriorityLevel::LOW},
    //     {"P3", 3, 1, PriorityLevel::HIGH},   {"P4", 4, 2, PriorityLevel::MEDIUM},
    //     {"P5", 5, 1, PriorityLevel::LOW},    {"P6", 6, 2, PriorityLevel::HIGH},
    //     {"P7", 7, 1, PriorityLevel::MEDIUM}, {"P8", 8, 1, PriorityLevel::LOW}};

    // workload_mixed
    std::vector<Process> processList = {
        {"P1", 0, 20, PriorityLevel::HIGH}, {"P2", 10, 25, PriorityLevel::MEDIUM},
        {"P3", 10, 7, PriorityLevel::LOW},  {"P4", 15, 15, PriorityLevel::LOW},
        {"P5", 21, 4, PriorityLevel::LOW},  {"P6", 30, 9, PriorityLevel::LOW},
        {"P7", 35, 3, PriorityLevel::LOW},  {"P8", 37, 20, PriorityLevel::LOW}};

    // FCFS : P3, P2, P1
    // SJF : P1, P2, P3

    std::string data;
    bool running = true;

    while (running) {
        int selected = handleMenu();
        switch (selected) {
            case 1: {
                FCFS fcfs(processList);
                runScheduler(fcfs, data);
                break;
            }
            case 2: {
                SJF sjf(processList);
                runScheduler(sjf, data);
                break;
            }
            case 3: {
                SRTF srtf(processList);
                runScheduler(srtf, data);
                break;
            }
            case 4: {
                RR rr(processList);
                runScheduler(rr, data);
                break;
            }

            case 5: {
                RR rrd(processList, RRType::DYNAMIC);
                runScheduler(rrd, data);
                break;
            }

            case 6: {
                RR rrd2(processList, RRType::DYNAMIC2);
                runScheduler(rrd2, data);
                break;
            }

            case 7: {
                Priority priority(processList);
                runScheduler(priority, data);
                break;
            }

            case 8: {
                Multilevel mlq(processList);
                runScheduler(mlq, data);
                break;
            }

            case 9: {
                CFS cfs(processList);
                runScheduler(cfs, data);
                break;
            }

            case 10: {
                runAllSchedulers(processList, data);
                break;
            }

            case 0:
                running = false;
                break;
            default:
                std::cout << "Invalid option\n";
                break;
        }
    }

    if (EXPORT_TO_CSV) {
        std::ofstream file(FILE_NAME);
        file << "Algorithm,PID,ArrivalTime,BurstTime,CompletionTime,WaitingTime,TurnaroundTime\n";
        file << data;
        file.close();
    }

    return 0;
}
