#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <climits>
#include <queue>

using namespace std;

struct GanttChartEntry {
    int processID;
    int startTime;
    int endTime;
};

struct Process {
    int pid;
    int at;
    int bt;
    int priority;

    int remainingTime;
    int completionTime;
    int tat;
    int wt;

    bool isFinished;
};

bool cmpPID(const Process& a, const Process& b) {
    return a.pid < b.pid;
}

bool cmpArrival(const Process& a, const Process& b) {
    if (a.at == b.at) return a.pid < b.pid;
    return a.at < b.at;
}

class Scheduler {
private:
    vector<Process> originalp;
    vector<Process> p;
    int numProcesses;
    int timeQuantum;

    void resetProcesses() {
        p = originalp;
        for (int i = 0; i < numProcesses; i++) {
            p[i].remainingTime = p[i].bt;
            p[i].completionTime = 0;
            p[i].tat = 0;
            p[i].wt = 0;
            p[i].isFinished = false;
        }
    }

    void printStats(string title, bool showPriority = false) {
        cout << "\n--- " << title << " ---";

        double totalTurnaroundTime = 0;
        double totalWaitingTime = 0;

        cout << "\nPID\tAT\tBT";
        if (showPriority) cout << "\tPRI";
        cout << "\tCT\tTAT\tWT\n";
        cout << "--------------------------------------------------";
        if (showPriority) cout << "----";
        cout << "\n";

        for (int i = 0; i < numProcesses; i++) {
            cout << p[i].pid << "\t"
                 << p[i].at << "\t"
                 << p[i].bt << "\t";
            if (showPriority) cout << p[i].priority << "\t";
            cout << p[i].completionTime << "\t"
                 << p[i].tat << "\t"
                 << p[i].wt << "\n";
            
            totalTurnaroundTime += p[i].tat;
            totalWaitingTime += p[i].wt;
        }

        cout << "\nAvg TAT: " << fixed << setprecision(2) << totalTurnaroundTime / numProcesses << "\n";
        cout << "Avg WT: " << fixed << setprecision(2) << totalWaitingTime / numProcesses << "\n\n";
    }

    void printGanttChart(string title, vector<GanttChartEntry>& chart) {
        cout << "Gantt Chart (" << title << "):\n";
        if (chart.empty()) return;

        cout << "|";
        for (int i = 0; i < chart.size(); i++) {
            string pid_str = (chart[i].processID == -1) ? "IDLE" : "P" + to_string(chart[i].processID);
            cout << " " << pid_str << " |";
        }
        cout << "\n";

        cout << chart[0].startTime;
        for (int i = 0; i < chart.size(); i++) {
            string pid_str = (chart[i].processID == -1) ? "IDLE" : "P" + to_string(chart[i].processID);
            int width = pid_str.length() + 3; 
            cout << setw(width) << right << chart[i].endTime;
        }
        cout << "\n\n";
    }


public:
    void input() {
        cout << "Enter number of processes: ";
        cin >> numProcesses;
        
        cout << "Enter Time Quantum for Round Robin: ";
        cin >> timeQuantum;

        originalp.resize(numProcesses);
        for (int i = 0; i < numProcesses; i++) {
            originalp[i].pid = i + 1;
            cout << "Process " << i + 1 << " (AT BT PRI): ";
            cin >> originalp[i].at 
                >> originalp[i].bt 
                >> originalp[i].priority;
        }
        
        sort(originalp.begin(), originalp.end(), cmpPID);
    }

    void runFCFS() {
        resetProcesses();
        string title = "FCFS (Non-Preemptive)";
        vector<GanttChartEntry> chart;
        
        sort(p.begin(), p.end(), cmpArrival);

        int currentTime = 0;
        for (int i = 0; i < numProcesses; i++) {
            if (currentTime < p[i].at) {
                chart.push_back({-1, currentTime, p[i].at});
                currentTime = p[i].at;
            }
            
            chart.push_back({p[i].pid, currentTime, currentTime + p[i].bt});
            
            currentTime += p[i].bt;
            p[i].completionTime = currentTime;
            p[i].tat = p[i].completionTime - p[i].at;
            p[i].wt = p[i].tat - p[i].bt;
        }

        sort(p.begin(), p.end(), cmpPID);
        
        printGanttChart(title, chart);
        printStats(title);
    }

    void runSRTF() {
        resetProcesses();
        string title = "SJF (Preemptive) / SRTF";
        vector<GanttChartEntry> chart;
        
        int currentTime = 0;
        int completedProcesses = 0;

        while (completedProcesses < numProcesses) {
            int shortestIndex = -1;
            int shortestTime = INT_MAX;

            for (int i = 0; i < numProcesses; i++) {
                if (p[i].at <= currentTime && !p[i].isFinished && p[i].remainingTime < shortestTime) {
                    shortestTime = p[i].remainingTime;
                    shortestIndex = i;
                }
            }

            if (shortestIndex == -1) {
                chart.push_back({-1, currentTime, currentTime + 1});
                currentTime++;
            } else {
                chart.push_back({p[shortestIndex].pid, currentTime, currentTime + 1});
                
                p[shortestIndex].remainingTime--;
                currentTime++;

                if (p[shortestIndex].remainingTime == 0) {
                    p[shortestIndex].isFinished = true;
                    completedProcesses++;
                    p[shortestIndex].completionTime = currentTime;
                    p[shortestIndex].tat = p[shortestIndex].completionTime - p[shortestIndex].at;
                    p[shortestIndex].wt = p[shortestIndex].tat - p[shortestIndex].bt;
                }
            }
        }
        
        printGanttChart(title, chart);
        printStats(title);
    }

    void runRoundRobin() {
        resetProcesses();
        string title = "Round Robin (TQ=" + to_string(timeQuantum) + ")";
        vector<GanttChartEntry> chart;

        vector<Process> p_sorted = p;
        sort(p_sorted.begin(), p_sorted.end(), cmpArrival);

        queue<int> readyQueue;
        int currentTime = 0;
        int completedProcesses = 0;
        int processIndex = 0;

        currentTime = p_sorted[0].at;
        readyQueue.push(p_sorted[processIndex++].pid - 1); 

        while (completedProcesses < numProcesses) {
            if (readyQueue.empty()) {
                if (processIndex < numProcesses) {
                    chart.push_back({-1, currentTime, p_sorted[processIndex].at});
                    currentTime = p_sorted[processIndex].at;
                    readyQueue.push(p_sorted[processIndex++].pid - 1);
                } else {
                    break;
                }
            }

            int currentPID_idx = readyQueue.front();
            readyQueue.pop();

            int executionTime = min(timeQuantum, p[currentPID_idx].remainingTime);
            chart.push_back({p[currentPID_idx].pid, currentTime, currentTime + executionTime});
            
            p[currentPID_idx].remainingTime -= executionTime;
            currentTime += executionTime;

            while (processIndex < numProcesses && p_sorted[processIndex].at <= currentTime) {
                readyQueue.push(p_sorted[processIndex++].pid - 1);
            }

            if (p[currentPID_idx].remainingTime > 0) {
                readyQueue.push(currentPID_idx);
            } else {
                completedProcesses++;
                p[currentPID_idx].completionTime = currentTime;
                p[currentPID_idx].tat = p[currentPID_idx].completionTime - p[currentPID_idx].at;
                p[currentPID_idx].wt = p[currentPID_idx].tat - p[currentPID_idx].bt;
            }
        }

        printGanttChart(title, chart);
        printStats(title);
    }

    void runPriorityNonPreemptive() {
        resetProcesses();
        string title = "Priority (Non-Preemptive)";
        vector<GanttChartEntry> chart;

        int currentTime = 0;
        int completedProcesses = 0;

        while (completedProcesses < numProcesses) {
            int highestPriorityIndex = -1;
            int highestPriority = INT_MAX;

            for (int i = 0; i < numProcesses; i++) {
                if (p[i].at <= currentTime && !p[i].isFinished && p[i].priority < highestPriority) {
                    highestPriority = p[i].priority;
                    highestPriorityIndex = i;
                }
            }

            if (highestPriorityIndex == -1) {
                chart.push_back({-1, currentTime, currentTime + 1});
                currentTime++;
            } else {
                int idx = highestPriorityIndex;
                chart.push_back({p[idx].pid, currentTime, currentTime + p[idx].bt});
                
                currentTime += p[idx].bt;
                
                p[idx].isFinished = true;
                completedProcesses++;
                p[idx].completionTime = currentTime;
                p[idx].tat = p[idx].completionTime - p[idx].at;
                p[idx].wt = p[idx].tat - p[idx].bt;
            }
        }
        
        printGanttChart(title, chart);
        printStats(title, true);
    }
};

int main() {
    Scheduler scheduler;
    scheduler.input();

    int choice;
    while(true) {
        cout << "\n--- Scheduler Menu ---\n";
        cout << "1. FCFS\n";
        cout << "2. SRTF (Preemptive SJF)\n";
        cout << "3. Round Robin\n";
        cout << "4. Priority (Non-Preemptive)\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            scheduler.runFCFS();
        } else if (choice == 2) {
            scheduler.runSRTF();
        } else if (choice == 3) {
            scheduler.runRoundRobin();
        } else if (choice == 4) {
            scheduler.runPriorityNonPreemptive();
        } else if (choice == 5) {
            break;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}