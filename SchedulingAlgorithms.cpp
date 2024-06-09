#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>
#include <sstream>

using namespace std;

// Define a structure to represent a process
struct Process {
    int id;             // Process ID
    int burstTime;      // Burst Time of the process
    int priority;       // Priority of the process
    int arrivalTime;    // Arrival Time of the process
    int waitingTime;    // Waiting Time of the process
    int turnAroundTime; // Turn Around Time of the process
    int remainingTime;  // Remaining Burst Time of the process
};

// Function to calculate waiting time for FCFS scheduling
void calculateWaitingTimeFCFS(vector<Process>& processes) {
    int n = processes.size();
    processes[0].waitingTime = 0;  // The first process has no waiting time

    // Calculate waiting time for each subsequent process
    for (int i = 1; i < n; i++) {
        processes[i].waitingTime = processes[i - 1].waitingTime + processes[i - 1].burstTime - processes[i].arrivalTime;
        if (processes[i].waitingTime < 0) {
            processes[i].waitingTime = 0;  // Ensure waiting time is not negative
        }
    }
}

// Function to calculate waiting time for Round Robin scheduling
void calculateWaitingTimeRR(vector<Process>& processes, int quantum) {
    int n = processes.size();
    vector<int> remainingBurstTime(n);
    for (int i = 0; i < n; i++) remainingBurstTime[i] = processes[i].burstTime;

    int time = 0;
    queue<int> q;
    for (int i = 0; i < n; i++) q.push(i);

    while (!q.empty()) {
        int i = q.front();
        q.pop();

        if (remainingBurstTime[i] > quantum) {
            time += quantum;
            remainingBurstTime[i] -= quantum;
            q.push(i);
        } else {
            time += remainingBurstTime[i];
            processes[i].waitingTime = time - processes[i].burstTime;
            remainingBurstTime[i] = 0;
        }
    }
}

// Function to calculate waiting time for Shortest Process Next scheduling
void calculateWaitingTimeSPN(vector<Process>& processes) {
    int n = processes.size();
    vector<bool> completed(n, false);
    int time = 0, completedProcesses = 0;

    while (completedProcesses < n) {
        int minIndex = -1;
        for (int i = 0; i < n; i++) {
            if (!completed[i] && processes[i].arrivalTime <= time) {
                if (minIndex == -1 || processes[i].burstTime < processes[minIndex].burstTime) {
                    minIndex = i;
                }
            }
        }

        if (minIndex != -1) {
            processes[minIndex].waitingTime = time - processes[minIndex].arrivalTime;
            time += processes[minIndex].burstTime;
            completed[minIndex] = true;
            completedProcesses++;
        } else {
            time++;
        }
    }
}

// Function to calculate waiting time for Shortest Remaining Time scheduling
void calculateWaitingTimeSRT(vector<Process>& processes) {
    int n = processes.size();
    int time = 0, completedProcesses = 0;
    vector<int> remainingBurstTime(n);
    for (int i = 0; i < n; i++) remainingBurstTime[i] = processes[i].burstTime;

    while (completedProcesses < n) {
        int minIndex = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= time && remainingBurstTime[i] > 0) {
                if (minIndex == -1 || remainingBurstTime[i] < remainingBurstTime[minIndex]) {
                    minIndex = i;
                }
            }
        }

        if (minIndex != -1) {
            remainingBurstTime[minIndex]--;
            if (remainingBurstTime[minIndex] == 0) {
                completedProcesses++;
                processes[minIndex].waitingTime = time + 1 - processes[minIndex].burstTime - processes[minIndex].arrivalTime;
            }
            time++;
        } else {
            time++;
        }
    }
}

// Function to calculate waiting time for Highest Response Ratio Next scheduling
void calculateWaitingTimeHRRN(vector<Process>& processes) {
    int n = processes.size();
    vector<bool> completed(n, false);
    int time = 0, completedProcesses = 0;

    while (completedProcesses < n) {
        int maxIndex = -1;
        double maxRatio = -1;
        for (int i = 0; i < n; i++) {
            if (!completed[i] && processes[i].arrivalTime <= time) {
                double responseRatio = (double)(time - processes[i].arrivalTime + processes[i].burstTime) / processes[i].burstTime;
                if (responseRatio > maxRatio) {
                    maxRatio = responseRatio;
                    maxIndex = i;
                }
            }
        }

        if (maxIndex != -1) {
            processes[maxIndex].waitingTime = time - processes[maxIndex].arrivalTime;
            time += processes[maxIndex].burstTime;
            completed[maxIndex] = true;
            completedProcesses++;
        } else {
            time++;
        }
    }
}

// Function to calculate waiting time for Feedback scheduling
void calculateWaitingTimeFB(vector<Process>& processes, int quantum) {
    int n = processes.size();
    vector<int> remainingBurstTime(n);
    vector<int> level(n, 0);
    for (int i = 0; i < n; i++) remainingBurstTime[i] = processes[i].burstTime;

    int time = 0;
    queue<int> q;
    for (int i = 0; i < n; i++) q.push(i);

    while (!q.empty()) {
        int i = q.front();
        q.pop();

        int currQuantum = quantum * (1 << level[i]);

        if (remainingBurstTime[i] > currQuantum) {
            remainingBurstTime[i] -= currQuantum;
            time += currQuantum;
            level[i]++;
            q.push(i);
        } else {
            time += remainingBurstTime[i];
            processes[i].waitingTime = time - processes[i].burstTime;
            remainingBurstTime[i] = 0;
        }
    }
}

// Function to calculate waiting time for Feedback with Varying Time Quantum scheduling
void calculateWaitingTimeFBV(vector<Process>& processes, vector<int> quanta) {
    int n = processes.size();
    vector<int> remainingBurstTime(n);
    vector<int> level(n, 0);
    for (int i = 0; i < n; i++) remainingBurstTime[i] = processes[i].burstTime;

    int time = 0;
    queue<int> q;
    for (int i = 0; i < n; i++) q.push(i);

    while (!q.empty()) {
        int i = q.front();
        q.pop();

        int currQuantum = quanta[level[i] % quanta.size()];

        if (remainingBurstTime[i] > currQuantum) {
            remainingBurstTime[i] -= currQuantum;
            time += currQuantum;
            level[i]++;
            q.push(i);
        } else {
            time += remainingBurstTime[i];
            processes[i].waitingTime = time - processes[i].burstTime;
            remainingBurstTime[i] = 0;
        }
    }
}

// Function to calculate waiting time for Aging scheduling
void calculateWaitingTimeAging(vector<Process>& processes, int agingInterval) {
    int n = processes.size();
    vector<int> remainingBurstTime(n);
    for (int i = 0; i < n; i++) remainingBurstTime[i] = processes[i].burstTime;

    int time = 0, completedProcesses = 0;

    while (completedProcesses < n) {
        int minIndex = -1;
        for (int i = 0; i < n; i++) {
            if (remainingBurstTime[i] > 0 && (minIndex == -1 || processes[i].priority < processes[minIndex].priority)) {
                minIndex = i;
            }
        }

        if (minIndex != -1) {
            processes[minIndex].waitingTime = time - processes[minIndex].arrivalTime;
            time += remainingBurstTime[minIndex];
            remainingBurstTime[minIndex] = 0;
            completedProcesses++;

            // Aging: Increase priority of other processes
            for (int i = 0; i < n; i++) {
                if (remainingBurstTime[i] > 0) {
                    processes[i].priority = max(0, processes[i].priority - agingInterval);
                }
            }
        } else {
            time++;
        }
    }
}

// Function to display results for a scheduling algorithm
float displayResults(vector<Process>& processes, ostringstream& oss) {
    int totalWaitingTime = 0;
    oss << "Process ID\tBurst Time\tPriority\tArrival Time\tWaiting Time\n";
    for (const auto& process : processes) {
        totalWaitingTime += process.waitingTime;
        oss << process.id << "\t\t" << process.burstTime << "\t\t" << process.priority << "\t\t" << process.arrivalTime << "\t\t" << process.waitingTime << "\n";
    }
    float avgWaitingTime = (float)totalWaitingTime / processes.size();
    oss << "Average Waiting Time: " << avgWaitingTime << "\n";
    return avgWaitingTime;
}

// Function to take input for processes from user
void inputProcesses(vector<Process>& processes) {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;
    processes.resize(n);

    cout << "Enter the burst time, priority, and arrival time for each process (space-separated):\n";
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "Process " << processes[i].id << ": ";
        cin >> processes[i].burstTime >> processes[i].priority >> processes[i].arrivalTime;
        processes[i].waitingTime = 0;
        processes[i].turnAroundTime = 0;
        processes[i].remainingTime = processes[i].burstTime;
    }
}

// Main function
int main() {
    int quantum, numQuanta, agingInterval;
    vector<int> quanta;
    vector<Process> processes;

    inputProcesses(processes);

    // Clone the processes list for each algorithm
    vector<Process> processesFCFS = processes;
    vector<Process> processesRR = processes;
    vector<Process> processesSPN = processes;
    vector<Process> processesSRT = processes;
    vector<Process> processesHRRN = processes;
    vector<Process> processesFB = processes;
    vector<Process> processesFBV = processes;
    vector<Process> processesAging = processes;

    ostringstream oss;
    // First Come First Serve (FCFS)
    oss << "First Come First Serve (FCFS):\n";
    calculateWaitingTimeFCFS(processesFCFS);
    float avgWaitingTimeFCFS = displayResults(processesFCFS, oss);

    // Round Robin (RR)
    oss << "\nRound Robin (RR):\n";
    cout << "Enter quantum for Round Robin: ";
    cin >> quantum;
    calculateWaitingTimeRR(processesRR, quantum);
    float avgWaitingTimeRR = displayResults(processesRR, oss);

    // Shortest Process Next (SPN)
    oss << "\nShortest Process Next (SPN):\n";
    calculateWaitingTimeSPN(processesSPN);
    float avgWaitingTimeSPN = displayResults(processesSPN, oss);

    // Shortest Remaining Time (SRT)
    oss << "\nShortest Remaining Time (SRT):\n";
    calculateWaitingTimeSRT(processesSRT);
    float avgWaitingTimeSRT = displayResults(processesSRT, oss);

    // Highest Response Ratio Next (HRRN)
    oss << "\nHighest Response Ratio Next (HRRN):\n";
    calculateWaitingTimeHRRN(processesHRRN);
    float avgWaitingTimeHRRN = displayResults(processesHRRN, oss);

    // Feedback (FB)
    oss << "\nFeedback (FB):\n";
    calculateWaitingTimeFB(processesFB, quantum);
    float avgWaitingTimeFB = displayResults(processesFB, oss);

    // Feedback with Varying Time Quantum (FBV)
    oss << "\nFeedback with Varying Time Quantum (FBV):\n";
    cout << "Enter the number of quanta for FBV: ";
    cin >> numQuanta;
    quanta.resize(numQuanta);
    for (int i = 0; i < numQuanta; i++) {
        cout << "Enter quantum " << i + 1 << ": ";
        cin >> quanta[i];
    }
    calculateWaitingTimeFBV(processesFBV, quanta);
    float avgWaitingTimeFBV = displayResults(processesFBV, oss);

    // Aging
    oss << "\nAging:\n";
    cout << "Enter aging interval: ";
    cin >> agingInterval;
    calculateWaitingTimeAging(processesAging, agingInterval);
    float avgWaitingTimeAging = displayResults(processesAging, oss);
    cout<<endl;
    // Display all waiting times together
    cout << "\nALL WAITING TIMES:\n" << oss.str() << endl;
    cout<<endl;
    // Determine the best algorithm based on average waiting time
    vector<pair<string, float>> results = {
        {"FCFS", avgWaitingTimeFCFS},
        {"RR", avgWaitingTimeRR},
        {"SPN", avgWaitingTimeSPN},
        {"SRT", avgWaitingTimeSRT},
        {"HRRN", avgWaitingTimeHRRN},
        {"FB", avgWaitingTimeFB},
        {"FBV", avgWaitingTimeFBV},
        {"Aging", avgWaitingTimeAging}
    };

    auto bestAlgorithm = min_element(results.begin(), results.end(), [](const pair<string, float>& a, const pair<string, float>& b) {
        return a.second < b.second;
    });
    cout<<"Average waiting time for First Come First Serve (FCFS) is " <<displayResults(processesFCFS, oss)<<endl;
    cout<<"Average waiting time for Round Robin (RR) with number of quantum "<<quantum<<" is " <<displayResults(processesRR, oss)<<endl;
    cout<<"Average waiting time for Shortest Process Next (SPN) is " <<displayResults(processesSPN, oss)<<endl;
    cout<<"Average waiting time for Shortest Remaining Time (SRT) is " <<displayResults(processesSRT, oss)<<endl;
    cout<<"Average waiting time for Highest Response Ratio Next (HRRN) is " <<displayResults(processesHRRN, oss)<<endl;
    cout<<"Average waiting time for Feedback (FB) is " <<displayResults(processesFB, oss)<<endl;
    cout<<"Average waiting time for Feedback with Varying Time Quantum (FBV) with "<<numQuanta<<" number of quanta is " <<displayResults(processesFBV, oss)<<endl;
    cout<<"Average waiting time for Aging with aging interval "<<agingInterval<<" is " <<displayResults(processesAging, oss)<<endl;
    cout<<endl;
    cout << "Best Algorithm: " << bestAlgorithm->first << " with Average Waiting Time: " << "'"<<bestAlgorithm->second <<"'"<< endl;

    return 0;
}
