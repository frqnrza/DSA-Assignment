//Muhammad Furqan Raza
//460535
//C++ based Process Manager

#include <iostream>
using namespace std ;

class Process {
public:
    int process_Id;
    int execution_time;
    int remaining_time;
    Process * next;

    Process(int id, int time) : process_Id(id), execution_time(time), remaining_time(time), next(nullptr) {}
};


class ProcessScheduler {
private:
    Process* head;
    const int cpu_time_per_cycle = 3;

public:
    ProcessScheduler() : head(nullptr)
    {
    }


    void AddProcess(int process_Id, int execution_time) {
        Process* newProcess = new Process(process_Id, execution_time);
        if (!head) {
            head = newProcess;
            newProcess->next = head;
        } else {
            Process* current = head;
            while (current->next != head) {
                current = current->next;
            }
            current->next = newProcess;
            newProcess->next = head;
        }
    }


    void RunCycle(int& cycleCount) {
        if (!head) {
            return;
        }

        cout << " Cycle " << cycleCount << " : " << endl;
        cout << " Before : ";
        displayState();

        Process* current = head;
        Process* prev = nullptr;
        Process* completedHead = nullptr;

        do {
            current->remaining_time -= cpu_time_per_cycle;
            if (current->remaining_time <= 0) {
                // Add to completed list
                Process* completed = new Process(current->process_Id, 0);
                completed->next = completedHead;
                completedHead = completed;

                // Remove from the circular list
                if (current->next == current) { // Last process
                    delete current;
                    head = nullptr;
                } else {
                    if (prev) {
                        prev->next = current->next;
                    } else {
                        head = current->next;
                    }
                    Process* temp = current;
                    current = current->next;
                    delete temp;
                    continue;
                }
            } else {
                prev = current;
            }

            if (head == nullptr) break;
            current = current->next;
        } while (current != head);

        cout << " After :  ";
        displayState();

        displayCompleted(completedHead);
        freeCompletedList(completedHead);

        cycleCount++;
        cout<<endl;
    }

    void displayState() {
        if (!head) {
            cout << " System Empty ! No processes in the System." <<endl;
            return;
        }

        Process* current = head;
        do {
            cout << "P" << current->process_Id << " ( Remaining: " << current->remaining_time << " ) ";
            current = current->next;
        } while (current != head);
        cout << endl;
    }

    void displayCompleted(Process* completed) {
        if (!completed) {
            return;
        }
        cout << " Completed Processes : ";
        Process* current = completed;
        while (current) {
            cout << "P" << current->process_Id;
            if (current->next) cout << ", ";
            current = current->next;
        }
        cout << endl;
    }

    void freeCompletedList(Process* completedHead) {
        while (completedHead) {
            Process* temp = completedHead;
            completedHead = completedHead->next;
            delete temp;
        }
    }

    void Simulate() {
        int cycleCount = 1;
        while (head) {
            RunCycle(cycleCount);
        }
    }

    ~ProcessScheduler() {
        if (!head) return;
        Process* current = head->next;
        while (current != head) {
            Process* temp = current;
            current = current->next;
            delete temp;
        }
        delete head;
    }
};


int main() {
    cout<<"\n\t\t Welcome to C++ Based Process Scheduler \n"<< endl ;
    ProcessScheduler scheduler;

    // Create processes
    scheduler.AddProcess(1, 12);
    scheduler.AddProcess(2, 7);
    scheduler.AddProcess(3, 9);

    cout << " \tSimulation Running : " << endl;
    scheduler.Simulate();

    // Adding a new process
    cout << " New Process Added !" << endl;
    scheduler.AddProcess(4, 6);
    scheduler.Simulate();
    cout << "\n\t\tThanks for using our System ! " << endl;
    return 0;
}