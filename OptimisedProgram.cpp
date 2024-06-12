#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <unordered_map>
using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    int startTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
};

// table header
void printHeader() {
    cout << setw(5) << "ID" 
         << setw(15) << "Arrival Time" 
         << setw(15) << "Burst Time" 
         << setw(15) << "Priority" 
         << setw(15) << "Start Time" 
         << setw(20) << "Completion Time" 
         << setw(20) << "Waiting Time" 
         << setw(20) << "Turnaround Time" 
         << endl;
    cout << string(120, '-') << endl;
}

// process table
void printProcess(const Process& p) {
    cout << setw(5) << p.id 
         << setw(15) << p.arrivalTime 
         << setw(15) << p.burstTime 
         << setw(15) << p.priority 
         << setw(15) << p.startTime 
         << setw(20) << p.completionTime 
         << setw(20) << p.waitingTime 
         << setw(20) << p.turnaroundTime 
         << endl;
}

//average waiting and turnaround times
void printAverages(const vector<Process>& processes) {
    double totalWaitingTime = 0, totalTurnaroundTime = 0;
    for (const auto& p : processes) {
        totalWaitingTime += p.waitingTime;
        totalTurnaroundTime += p.turnaroundTime;
    }
    cout << "Average Waiting Time: " << totalWaitingTime / processes.size() << endl;
    cout << "Average Turnaround Time: " << totalTurnaroundTime / processes.size() << endl;
}

// First-Come, First-Served 
void fcfs(vector<Process>& processes) {
    cout << "\nFirst-Come, First-Served Scheduling:\n";
    printHeader();

    int currentTime = 0;
    for (auto& p : processes) {
        p.startTime = max(currentTime, p.arrivalTime);
        p.completionTime = p.startTime + p.burstTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        currentTime = p.completionTime;
        printProcess(p);
    }
    printAverages(processes);
}

// Shortest Job Next
void sjn(vector<Process>& processes) {
    cout << "\nShortest Job Next Scheduling:\n";
    printHeader();

    auto compareBurstTime = [](Process a, Process b) { return a.burstTime > b.burstTime; };
    priority_queue<Process, vector<Process>, decltype(compareBurstTime)> pq(compareBurstTime);

    int currentTime = 0;
    int completed = 0;
    unordered_map<int, bool> isInQueue;

    while (completed < processes.size()) {
        for (auto& p : processes) {
            if (p.arrivalTime <= currentTime && !isInQueue[p.id]) {
                pq.push(p);
                isInQueue[p.id] = true;
            }
        }

        if (!pq.empty()) {
            auto p = pq.top();
            pq.pop();

            p.startTime = currentTime;
            p.completionTime = p.startTime + p.burstTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.turnaroundTime - p.burstTime;
            currentTime = p.completionTime;

            processes[p.id - 1] = p; 
            printProcess(p);
            completed++;
        } else {
            currentTime++;
        }
    }
    printAverages(processes);
}


// Priority-Based Scheduling
void priorityScheduling(vector<Process>& processes) {
    cout << "\nPriority-Based Scheduling:\n";
    printHeader();

    auto comparePriority = [](Process a, Process b) { return a.priority > b.priority; };
    priority_queue<Process, vector<Process>, decltype(comparePriority)> pq(comparePriority);

    int currentTime = 0;
    int completed = 0;
    unordered_map<int, bool> isInQueue;

    while (completed < processes.size()) {
        for (auto& p : processes) {
            if (p.arrivalTime <= currentTime && !isInQueue[p.id]) {
                pq.push(p);
                isInQueue[p.id] = true;
            }
        }

        if (!pq.empty()) {
            auto p = pq.top();
            pq.pop();

            p.startTime = currentTime;
            p.completionTime = p.startTime + p.burstTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.turnaroundTime - p.burstTime;
            currentTime = p.completionTime;

            processes[p.id - 1] = p;  
            printProcess(p);
            completed++;
        } else {
            currentTime++;
        }
    }
    printAverages(processes);
}


// Shortest Remaining Time (SRT)
void srt(vector<Process>& processes) {
    cout << "\nShortest Remaining Time Scheduling:\n";
    printHeader();

    auto compareRemainingTime = [](Process a, Process b) { return a.remainingTime > b.remainingTime; };
    priority_queue<Process, vector<Process>, decltype(compareRemainingTime)> pq(compareRemainingTime);

    int currentTime = 0;
    int completed = 0;
    unordered_map<int, bool> isInQueue;

    while (completed < processes.size()) {
        for (auto& p : processes) {
            if (p.arrivalTime <= currentTime && !isInQueue[p.id]) {
                pq.push(p);
                isInQueue[p.id] = true;
            }
        }

        if (!pq.empty()) {
            auto p = pq.top();
            pq.pop();

            if (p.startTime == -1) {
                p.startTime = currentTime;
            }

            currentTime++;
            p.remainingTime--;

            if (p.remainingTime == 0) {
                p.completionTime = currentTime;
                p.turnaroundTime = p.completionTime - p.arrivalTime;
                p.waitingTime = p.turnaroundTime - p.burstTime;
                processes[p.id - 1] = p;  
                printProcess(p);
                completed++;
            } else {
                pq.push(p);  
            }
        } else {
            currentTime++;
        }
    }
    printAverages(processes);
}


// Round Robin (RR)
void roundRobin(vector<Process>& processes, int timeQuantum) {
    cout << "\nRound Robin Scheduling (Time Quantum = " << timeQuantum << "):\n";
    printHeader();

    queue<Process> q;
    int currentTime = 0;
    int completed = 0;
    vector<bool> isInQueue(processes.size(), false);

    while (completed < processes.size()) {
        for (auto& p : processes) {
            if (p.arrivalTime <= currentTime && !isInQueue[p.id - 1]) {
                q.push(p);
                isInQueue[p.id - 1] = true;
            }
        }

        if (!q.empty()) {
            auto p = q.front();
            q.pop();

            if (p.startTime == -1) {
                p.startTime = currentTime;
            }

            int execTime = min(timeQuantum, p.remainingTime);
            currentTime += execTime;
            p.remainingTime -= execTime;

            if (p.remainingTime == 0) {
                p.completionTime = currentTime;
                p.turnaroundTime = p.completionTime - p.arrivalTime;
                p.waitingTime = p.turnaroundTime - p.burstTime;
                processes[p.id - 1] = p; 
                printProcess(p);
                completed++;
            } else {
                q.push(p);  
            }
        } else {
            currentTime++;
        }
    }
    printAverages(processes);
}

// Highest Response Ratio Next (HRRN)
void hrrn(vector<Process>& processes) {
    cout << "\nHighest Response Ratio Next Scheduling:\n";
    printHeader();

    auto compareResponseRatio = [](Process a, Process b) {
        double responseRatioA = (a.waitingTime + a.burstTime) / static_cast<double>(a.burstTime);
        double responseRatioB = (b.waitingTime + b.burstTime) / static_cast<double>(b.burstTime);
        return responseRatioA < responseRatioB;
    };
    priority_queue<Process, vector<Process>, decltype(compareResponseRatio)> pq(compareResponseRatio);

    int currentTime = 0;
    int completed = 0;
    unordered_map<int, bool> isInQueue;

    while (completed < processes.size()) {
        for (auto& p : processes) {
            if (p.arrivalTime <= currentTime && !isInQueue[p.id]) {
                p.waitingTime = currentTime - p.arrivalTime;
                pq.push(p);
                isInQueue[p.id] = true;
            }
        }

        if (!pq.empty()) {
            auto p = pq.top();
            pq.pop();

            p.startTime = currentTime;
            p.completionTime = p.startTime + p.burstTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.turnaroundTime - p.burstTime;
            currentTime = p.completionTime;

            processes[p.id - 1] = p;  
            printProcess(p);
            completed++;
        } else {
            currentTime++;
        }
    }
    printAverages(processes);
}

int main() {
    vector<Process> processes = {
        {1, 0, 5, 5, 2, -1, -1, -1, -1},
        {2, 1, 3, 3, 1, -1, -1, -1, -1},
        {3, 2, 8, 8, 4, -1, -1, -1, -1},
        {4, 3, 6, 6, 3, -1, -1, -1, -1}
    };

    vector<Process> processesFCFS = processes;
    fcfs(processesFCFS);

    vector<Process> processesSJN = processes;
    sjn(processesSJN);

    vector<Process> processesPriority = processes;
    priorityScheduling(processesPriority);

    vector<Process> processesSRT = processes;
    srt(processesSRT);

    vector<Process> processesRR = processes;
    roundRobin(processesRR, 2);

    vector<Process> processesHRRN = processes;
    hrrn(processesHRRN);

    return 0;
}
